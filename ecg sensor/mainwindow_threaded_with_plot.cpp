#include "mainwindow_threaded_with_plot.h"
#include <QVBoxLayout>
#include <QThread>
#include <QDebug>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <QtCharts/QChartView>
#include <QPainter>
QT_CHARTS_USE_NAMESPACE


// read date 
class HeartWorker : public QObject {
    Q_OBJECT

public:
    explicit HeartWorker(QObject* parent = nullptr)
        : QObject(parent), bufferIndex(0) {
    }

    void setDevice(int dev) { i2cFile = dev; }

signals:
    void newBPMAvailable(float bpm);
    void newIRSample(int value);

public slots:
    void startWorking() {
        while (true) {
            int32_t red, ir;
            max30102_read_fifo(i2cFile, &red, &ir);
            emit newIRSample(ir);
            irBuffer[bufferIndex++] = ir;

            if (bufferIndex >= 200) {
                low_pass_filter(irBuffer, 200);
                float bpm = calculate_heart_rate(irBuffer, 200);
                bpm = (bpm != 0) ? bpm * 1.8f : 0;
                emit newBPMAvailable(bpm);
                bufferIndex = 0;
            }
            QThread::msleep(5);
        }
    }

private:
    int i2cFile;
    int32_t irBuffer[200];
    int bufferIndex;

    void max30102_read_fifo(int file, int32_t* red_value, int32_t* ir_value) {
        uint8_t buffer[6];
        i2c_read(file, 0x07, buffer, 6);
        *red_value = (buffer[0] << 16) | (buffer[1] << 8) | buffer[2];
        *ir_value = (buffer[3] << 16) | (buffer[4] << 8) | buffer[5];
    }
    void low_pass_filter(int32_t* buffer, int length) {
        float alpha = 0.1f;
        for (int i = 1; i < length; ++i)
            buffer[i] = alpha * buffer[i] + (1 - alpha) * buffer[i - 1];
    }
    float calculate_heart_rate(int32_t* buf, int len) {
        int peaks = 0, threshold = 1000;
        for (int i = 1; i < len - 1; ++i) {
            if (buf[i] > buf[i - 1] && buf[i] > buf[i + 1] && buf[i] > threshold)
                ++peaks;
        }
        return (peaks / float(len)) * 60.0f * 2.0f;
    }
    int i2c_read(int file, uint8_t reg, uint8_t* b, int len) {
        if (::write(file, &reg, 1) != 1) return -1;
        if (::read(file, b, len) != len) return -1;
        return 0;
    }
};

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), xValue(0)
{
    // record start time
    startTime = QDateTime::currentMSecsSinceEpoch();
    tenSecondsProcessed = false;

    
    QWidget* central = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(central);
    rateLabel = new QLabel("Heart Rate: -- BPM", this);
    statusLabel = new QLabel("Status: --", this);
    maxRateLabel = new QLabel("Rolling Max (10s): -- BPM", this);

    series = new QLineSeries();
    chart = new QChart();
    chart->addSeries(series);
    chart->legend()->hide();
    chart->setTitle("IR Signal");

    axisX = new QValueAxis;
    axisX->setRange(0, 2000);
    axisX->setTitleText("Samples");

    axisY = new QValueAxis;
    axisY->setRange(0, 100000);
    axisY->setTitleText("IR Value");

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisX);
    series->attachAxis(axisY);

    QChartView* chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    layout->addWidget(rateLabel);
    layout->addWidget(statusLabel);
    layout->addWidget(maxRateLabel);
    layout->addWidget(chartView);
    setCentralWidget(central);
    resize(600, 400);

    // open i2c
    if ((i2cFile = open("/dev/i2c-1", O_RDWR)) < 0 ||
        ioctl(i2cFile, I2C_SLAVE, 0x57) < 0) {
        qFatal("I2C åˆå§‹åŒ–å¤±è´¥");
    }
    initSensor();

    // start thread
    QThread* thread = new QThread(this);
    HeartWorker* worker = new HeartWorker;
    worker->setDevice(i2cFile);
    worker->moveToThread(thread);
    connect(thread, &QThread::started, worker, &HeartWorker::startWorking);
    connect(worker, &HeartWorker::newBPMAvailable, this, &MainWindow::updateBPM);
    connect(worker, &HeartWorker::newIRSample, this, &MainWindow::appendIRSample);
    thread->start();
}

void MainWindow::updateBPM(float bpm) {
    // renew state
    rateLabel->setText(QString("Heart Rate: %1 BPM").arg(bpm, 0, 'f', 2));
    statusLabel->setText(bpm > 100 ? "Status: lay" : "Status: normal");

    // time and date
    qint64 now = QDateTime::currentMSecsSinceEpoch();
    bpmHistory.append(qMakePair(now, bpm));

    // clean date calculate max date
    while (!bpmHistory.isEmpty() && bpmHistory.first().first < now - 10000)
        bpmHistory.removeFirst();
    float rollingMax = 0;
    for (auto& rec : bpmHistory)
        rollingMax = qMax(rollingMax, rec.second);
    maxRateLabel->setText(
        QString("Rolling Max (10s): %1 BPM").arg(rollingMax, 0, 'f', 2)
    );

    // 10s show one result
    if (!tenSecondsProcessed && now - startTime >= 10000) {
        tenSecondsProcessed = true;
        bool normal = (rollingMax <= 155.0f);
        QString finalText = QString("10s max:%1 BPM -” %2")
            .arg(rollingMax, 0, 'f', 2)
            .arg(normal ? "normal" : "abnormal");
        maxRateLabel->setText(finalText);
        qDebug() << finalText;
    }
}

void MainWindow::appendIRSample(int value) {
    static QVector<QPointF> buf;
    static int logicalX = 0;
    buf.append(QPointF(logicalX++, value));
    if (buf.size() > 600) buf.remove(0);
    series->replace(buf);
    axisX->setRange(buf.first().x(), buf.last().x());

    int minY = value, maxY = value;
    for (auto& pt : buf) {
        minY = qMin(minY, int(pt.y()));
        maxY = qMax(maxY, int(pt.y()));
    }
    if (maxY - minY < 100) {
        minY -= 50;
        maxY += 50;
    }
    axisY->setLabelFormat("%e");
    axisY->setRange(minY, maxY);
}

void MainWindow::initSensor() {
    i2c_write(i2cFile, 0x09, 0x40); usleep(10000);
    i2c_write(i2cFile, 0x09, 0x03);
    i2c_write(i2cFile, 0x0A, 0x27);
    i2c_write(i2cFile, 0x0C, 0x24);
    i2c_write(i2cFile, 0x0D, 0x24);
    i2c_write(i2cFile, 0x04, 0x00);
    i2c_write(i2cFile, 0x05, 0x00);
    i2c_write(i2cFile, 0x06, 0x00);
}

int MainWindow::i2c_write(int file, uint8_t reg, uint8_t value) {
    uint8_t buf[2] = { reg, value };
    return (::write(file, buf, 2) == 2) ? 0 : -1;
}

MainWindow::~MainWindow() {
    ::close(i2cFile);
}

#include "mainwindow_threaded_with_plot.moc"
