
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <dlib/opencv.h>
#include <QThread>
#include <QElapsedTimer>
#include <QLabel>
#include <QDebug>
#include <thread>
#include <vector>
#include <algorithm>
#include <QVBoxLayout>
#include <QChartView>
#include <QPainter>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
QT_CHARTS_USE_NAMESPACE

using namespace std;
using namespace cv;

const vector<string> EMOTION_LABELS = {"Angry", "Disgust", "Fear", "Happy", "Sad", "Surprise", "Neutral"};
const vector<string> CATEGORY_LABELS = {"Sad", "Happy", "Neutral"};
const array<int, 7> CATEGORY_MAPPING = {0, 0, 0, 1, 0, 1, 2};

class FrameGrabber : public QThread {
public:
    cv::VideoCapture cap;
    std::function<void(cv::Mat)> onNewFrame;
    std::string pipeline;
    bool stopFlag = false;

    void run() override {
        cap.open(pipeline, cv::CAP_GSTREAMER);
        if (!cap.isOpened()) {
            qDebug() << "cant open camera";
            return;
        }
        cv::Mat frame;
        while (!stopFlag) {
            cap.read(frame);
            if (!frame.empty() && onNewFrame) onNewFrame(frame.clone());
            QThread::msleep(30);
        }
    }

    void stop() {
        stopFlag = true;
        wait();
        cap.release();
    }
};

// heart thread
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
                bpm = (bpm != 0) ? bpm * 2.2f : 0;
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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow),
      ort_env(ORT_LOGGING_LEVEL_WARNING, "emotion"),
      ort_session(ort_env, "mobilenetv2-7.onnx", Ort::SessionOptions{}),
      isDetecting(false), frame_count(0), reported(false)
{
    ui->setupUi(this);
// chart
series = new QLineSeries(this);
chart = new QChart();
chart->addSeries(series);
chart->legend()->hide();
chart->setTitle("IR Signal");

axisX = new QValueAxis();
axisY = new QValueAxis();
axisX->setRange(0, 200);
axisY->setRange(0, 100000);
chart->addAxis(axisX, Qt::AlignBottom);
chart->addAxis(axisY, Qt::AlignLeft);
series->attachAxis(axisX);
series->attachAxis(axisY);

chartView = ui->chartView;
chartView->setChart(chart);
chartView->setRenderHint(QPainter::Antialiasing);

    i2cFile = ::open("/dev/i2c-1", O_RDWR);
    if (i2cFile < 0 || ioctl(i2cFile, I2C_SLAVE, 0x57) < 0) {
        qDebug() << "cant open MAX30102";
    } else {
        initSensor();

        heartThread = new QThread(this);
        worker = new HeartWorker();
        worker->setDevice(i2cFile);
        worker->moveToThread(heartThread);
        connect(heartThread, &QThread::started, worker, &HeartWorker::startWorking);
        connect(worker, &HeartWorker::newIRSample, this, &MainWindow::appendIRSample);
        connect(worker, &HeartWorker::newBPMAvailable, this, &MainWindow::updateBPM);
        heartThread->start();
    }


    resultLabel = new QLabel(this);
    rateLabel = new QLabel(this);
    statusLabel = new QLabel(this);
    maxRateLabel = new QLabel(this);
    resultLabel->setStyleSheet("font-size: 16px; color: blue;");
    resultLabel->setAlignment(Qt::AlignCenter);
    ui->leftLayout->addWidget(resultLabel);
    ui->rightLayout->addWidget(rateLabel);
    ui->rightLayout->addWidget(statusLabel);
    ui->rightLayout->addWidget(maxRateLabel);

    grabber = new FrameGrabber();
    grabber->pipeline =
        "libcamerasrc ! video/x-raw,format=RGB,width=640,height=480,framerate=30/1 ! "
        "videoconvert ! video/x-raw,format=BGR ! appsink";

    detector = dlib::get_frontal_face_detector();
    Ort::AllocatorWithDefaultOptions allocator;
    input_name = ort_session.GetInputNameAllocated(0, allocator).release();
    output_name = ort_session.GetOutputNameAllocated(0, allocator).release();

    connect(grabber, &QThread::finished, grabber, &QObject::deleteLater);
    grabber->onNewFrame = [this](cv::Mat frame) { this->handleCameraFrame(frame); };

    uiTimer = new QElapsedTimer();
    uiTimer->start();

    connect(ui->startButton, &QPushButton::clicked, this, &MainWindow::toggleDetection);
}

MainWindow::~MainWindow() {
    if (heartThread && heartThread->isRunning()) {
        heartThread->quit();
        heartThread->wait();
    }
    if (worker) {
        worker->deleteLater();
        worker = nullptr;
    }
    if (grabber && grabber->isRunning()) grabber->stop();
    if (i2cFile > 0) ::close(i2cFile);

    if (i2cFile > 0) ::close(i2cFile);
    if (grabber->isRunning()) grabber->stop();
    delete ui;
}

void MainWindow::toggleDetection() {
    isDetecting = !isDetecting;
    if (isDetecting) {
        start_time = chrono::steady_clock::now();
        category_counts.assign(3, 0);
        reported = false;
        frame_count = 0;
        resultLabel->clear();
        grabber->start();
        ui->startButton->setText("stop");
    } else {
        grabber->stop();
        ui->startButton->setText("start");
    }
}

void MainWindow::handleCameraFrame(cv::Mat frame) {
    if (!isDetecting) return;
    frame_count++;

    cv::Mat small_frame;
    cv::resize(frame, small_frame, cv::Size(), 0.5, 0.5);

    if (frame_count % 5 == 0) {
        cached_faces.clear();
        dlib::cv_image<dlib::bgr_pixel> dlib_img(small_frame);
        for (auto& f : detector(dlib_img)) {
            cv::Rect r(cv::Point(f.left()*2, f.top()*2), cv::Point(f.right()*2, f.bottom()*2));
            r &= cv::Rect(0, 0, frame.cols, frame.rows);
            cached_faces.push_back(r);
        }
        vector<int> emotion_indices = predictEmotions(frame, cached_faces);
        drawPredictions(frame, emotion_indices);
    }

    if (uiTimer->elapsed() > 100) {
        QImage image(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
        if (!image.isNull()) {
            ui->videoLabel->setPixmap(QPixmap::fromImage(image));
            ui->videoLabel->setScaledContents(true);
        }
        uiTimer->restart();
    }
}

vector<int> MainWindow::predictEmotions(const Mat& frame, const vector<Rect>& faces) {
    size_t n = faces.size();
    std::vector<int> results(n, 0);
    for (size_t i = 0; i < n; ++i) {
        Mat face = frame(faces[i]).clone();
        cv::resize(face, face, Size(224, 224));
        face.convertTo(face, CV_32F, 1.0 / 255.0);

        vector<float> input_tensor(3 * 224 * 224);
        for (int c = 0; c < 3; ++c)
            for (int y = 0; y < 224; ++y)
                for (int x = 0; x < 224; ++x)
                    input_tensor[c * 224 * 224 + y * 224 + x] = face.at<Vec3f>(y, x)[c];

        array<int64_t, 4> input_shape{1, 3, 224, 224};
        auto input_tensor_value = Ort::Value::CreateTensor<float>(
            Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault),
            input_tensor.data(), input_tensor.size(), input_shape.data(), input_shape.size());

        auto output = ort_session.Run(Ort::RunOptions{nullptr},
                                      &input_name, &input_tensor_value, 1,
                                      &output_name, 1);
        float* logits = output[0].GetTensorMutableData<float>();

        vector<float> probs(7);
        float sum = 0.0f;
        for (int j = 0; j < 7; ++j) sum += (probs[j] = exp(logits[j]));
        for (auto& p : probs) p /= sum;

        results[i] = max_element(probs.begin(), probs.end()) - probs.begin();
    }

    auto now = chrono::steady_clock::now();
    double elapsed = chrono::duration_cast<chrono::seconds>(now - start_time).count();
    for (int idx : results) {
        int cat = CATEGORY_MAPPING[idx];
        if (elapsed <= 5.0)
            category_counts[cat]++;
    }
    if (!reported && chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now() - start_time).count() > 20.0) {
        int top = max_element(category_counts.begin(), category_counts.end()) - category_counts.begin();
        QString resultText = "most emotion in 10s: " + QString::fromStdString(CATEGORY_LABELS[top]);
        resultLabel->setText(resultText);
        reported = true;
    }

    return results;
}

void MainWindow::drawPredictions(Mat& frame, const vector<int>& emotion_indices) {
    for (size_t i = 0; i < cached_faces.size(); ++i) {
        int cat = CATEGORY_MAPPING[emotion_indices[i]];
        putText(frame, CATEGORY_LABELS[cat], Point(cached_faces[i].x, cached_faces[i].y - 5),
                FONT_HERSHEY_SIMPLEX, 0.6, Scalar(0, 255, 0), 2);
        rectangle(frame, cached_faces[i], Scalar(0, 255, 0), 2);
    }
}

void MainWindow::updateBPM(float bpm) {
    // renew state
    rateLabel->setText(QString("Heart Rate: %1 BPM").arg(bpm, 0, 'f', 2));
    statusLabel->setText(bpm > 140 ? "Status: abnormal" : "Status: normal");

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

#include "mainwindow.moc"
