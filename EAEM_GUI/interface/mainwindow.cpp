#include "mainwindow.h"
#include <QDebug>
#include <QMessageBox>
#include <QtConcurrent/QtConcurrent>
#include <QFuture>
#include <QFutureWatcher>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    UI_SetUp();

    connect(dashboardWidget, &DashboardWidget::sig_emotion_detection_start, this, &MainWindow::on_pushbutton_emotion_detection_start_clicked);
    connect(dashboardWidget, &DashboardWidget::sig_emotion_detection_stop, this, &MainWindow::on_pushbutton_emotion_detection_stop_clicked);
    connect(this, &MainWindow::sig_sendSystemInfoToReplace, emotionIndicatorWidget, &EmotionIndicatorWidget::replace_textEditInfo_Display);
    connect(this, &MainWindow::sig_sendSysteminfoToAppend, emotionIndicatorWidget, &EmotionIndicatorWidget::append_textEditInfo_Display);
    connect(this, &MainWindow::sig_videoCaptureStart, videoDisplayWidget, &VideoDisplayWidget::initCamera);
}

MainWindow::~MainWindow()
{
    // No need to delete UI here, as it is handled by Qt's parent-child memory management
    qDebug() << "Windows Closed.";
}

void MainWindow::UI_SetUp()
{
    setWindowTitle("Emotion Detection System");
    resize(980, 1044);

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // Dashboard Module
    dashboardWidget = new DashboardWidget(this);
    mainLayout->addWidget(dashboardWidget,0);

    // Main Control Layout
    QHBoxLayout *horizontalLayout_Controler = new QHBoxLayout;

    // Video Display Module
    videoDisplayWidget = new VideoDisplayWidget(this);
    horizontalLayout_Controler->addWidget(videoDisplayWidget,1);

    // Right Side Layout for Emotion Indicator and Sensors
    QVBoxLayout *verticalLayout_Sensors = new QVBoxLayout;

    // Emotion Indicator Module
    emotionIndicatorWidget = new EmotionIndicatorWidget();
    verticalLayout_Sensors->addWidget(emotionIndicatorWidget,0);

    // MAX30102 Sensor Module
    sensorMAX30102Widget = new SensorMAX30102Widget(this);
    verticalLayout_Sensors->addWidget(sensorMAX30102Widget,1);

    // AHT20 Sensor Module
    sensorAHT20Widget = new SensorAHT20Widget(this);
    verticalLayout_Sensors->addWidget(sensorAHT20Widget,1);

    horizontalLayout_Controler->addLayout(verticalLayout_Sensors,1);
    mainLayout->addLayout(horizontalLayout_Controler,0);

    // ECG & HRV Module
    ecgHrvWidget = new EcgHrvWidget(this);
    mainLayout->addWidget(ecgHrvWidget,1);

    // Temperature & Humidity Module
    temperatureHumidityWidget = new TemperatureHumidityWidget(this);
    mainLayout->addWidget(temperatureHumidityWidget,1);

    // Set Layout and Central Widget
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);
    setStatusBar(new QStatusBar(this));
}

void MainWindow::on_pushbutton_emotion_detection_start_clicked()
{
    qDebug() << "System Start";

    QFutureWatcher<bool> *watcher = new QFutureWatcher<bool>(this);

    connect(watcher, &QFutureWatcher<bool>::finished, this, [=]() {
        bool success = watcher->future().result();

        if (success) {
            dashboardWidget->set_pushbuton_enable_start();
            emit sig_sendSysteminfoToAppend("Pigpio initialized successfully");
            emit sig_videoCaptureStart();
        }
        else {
            QMessageBox::critical(this, "GPIO ERROR", "Failed to initialize pigpio after 5 attempts.");
            dashboardWidget->set_pushbuton_enable_stop();
        }
        watcher->deleteLater();
    });

    QFuture<bool> future = QtConcurrent::run(this, &MainWindow::initialize_GPIO);
    watcher->setFuture(future);
}

void MainWindow::on_pushbutton_emotion_detection_stop_clicked()
{
    qDebug() << "System Stop";
}

bool MainWindow::initialize_GPIO()
{
    int retryCount = 0;
    const int maxRetries = 5;

    while (retryCount < maxRetries) {
        if (gpioInitialise() < 0) {
            QThread::sleep(1);
            retryCount++;
            emit sig_sendSystemInfoToReplace("Failed to initialize pigpio. Retrying " + QString::number(retryCount) + " attempts.");
            continue;
        }
        qDebug() << "Pigpio initialized successfully";
        return true;
    }

    qDebug() << "Failed to initialize pigpio after 5 attempts.";
    return false;
}
