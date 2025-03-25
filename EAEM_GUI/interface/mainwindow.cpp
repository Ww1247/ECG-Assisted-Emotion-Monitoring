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

    connect(this, &MainWindow::sig_videoCaptureStart, videoDisplayWidget, &VideoDisplayWidget::initCamera);
    connect(this, &MainWindow::sig_videoCaptureStop, videoDisplayWidget, &VideoDisplayWidget::stopCamera);

    connect(this, &MainWindow::sig_initSensorMAX30102, sensorMAX30102Widget, &SensorMAX30102Widget::sensorInit);

    connect(this, &MainWindow::sig_startSensorAHT20Thread, sensorAHT20Widget, &SensorAHT20Widget::startSensorThread);

    qAddPostRoutine([](){
        gpioTerminate();
        qDebug() << "[LOG]: gpioTerminate called at post-routine shutdown.";
    });
}

MainWindow::~MainWindow()
{
    // No need to delete UI here, as it is handled by Qt's parent-child memory management
    qDebug() << "[LOG]: System Stoped";
    qDebug() << "[LOG]: Windows Closed.";
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
    qDebug() << "[LOG]: System Start";
    run_GPIO_Initialize();
}

void MainWindow::on_pushbutton_emotion_detection_stop_clicked()
{
    display_info("append", "System Stop");
    emit sig_videoCaptureStop();
}

void MainWindow::run_GPIO_Initialize()
{
    QFutureWatcher<bool> *watcher = new QFutureWatcher<bool>(this);
    connect(watcher, &QFutureWatcher<bool>::finished, this, [=]() {
        bool success = watcher->future().result();
        if (success) {
            this->run_Camera_Initialize();
        }
        else {
            errorOccurred("GPIO ERROR", "Failed to initialize pigpio after 5 attempts.");
        }
        watcher->deleteLater();
    });
    QFuture<bool> future = QtConcurrent::run(this, &MainWindow::initialize_GPIO);
    watcher->setFuture(future);
}

void MainWindow::run_Camera_Initialize()
{
    qDebug() << "[LOG]: Open Camera ...";
    if (!initialize_Camera()) {
        qDebug() << "[ERROR]: Failed to initialize camera.";
        errorOccurred("CAMERA ERROR", "Failed to initialize camera.");
        return;
    }
    qDebug() << "[LOG]: Camera open successfully!";
    run_Sensor_Read();
}

void MainWindow::run_Sensor_Read()
{
    qDebug() << "[LOG]: Turn On Sensor Reading ...";
    if (!initialize_SensorReading()) {
        errorOccurred("SENSOR ERROR", "Failed to read form sensor.");
        return;
    }
    qDebug() << "[LOG]: Sensor reading started.";
}

bool MainWindow::initialize_GPIO()
{
    int retryCount = 0;
    const int maxRetries = 5;

    while (retryCount < maxRetries) {
        if (gpioInitialise() < 0) {
            QThread::sleep(1);
            retryCount++;
//            display_info("replace", "Failed to initialize pigpio. Retrying " + QString::number(retryCount) + " attempts.");
            qDebug() << "[ERROR]: Failed to initialize pigpio. Retrying " << retryCount << " attempts.";
            continue;
        }
//        display_info("append", "Pigpio initialized successfully!");
        qDebug() << "[LOG]: Pigpio initialized successfully!";
        return true;
    }
//    display_info("replace", "Failed to initialize pigpio after 5 attempts.");
    qDebug() << "[ERROR]: Failed to initialize pigpio after 5 attempts.";
    return false;
}

bool MainWindow::initialize_Camera()
{
    emit sig_videoCaptureStart(0);
    return true;
}

bool MainWindow::initialize_SensorReading()
{
//    QMetaObject::invokeMethod(sensorMAX30102Widget, "isSensorReady",
//                              Qt::BlockingQueuedConnection,
//                              Q_RETURN_ARG(bool, ready));
//    emit sig_initSensorMAX30102();
    emit sig_startSensorAHT20Thread();
    return true;
}

void MainWindow::errorOccurred(const QString &error_type, const QString &error_message)
{
    QMessageBox::critical(this, error_type, error_message);
    dashboardWidget->set_pushbuton_enable_stop();
    display_info("clear","");
    return;
}

QString MainWindow::display_info(const QString &function_select, const QString &info)
{
    if (function_select == "replace") {
        emotionIndicatorWidget->replace_textEditInfo_Display("[LOG]: " + info);
    }
    else if (function_select == "append") {
        emotionIndicatorWidget->append_textEditInfo_Display("[LOG]: " + info);
    }
    else if (function_select == "error") {
        emotionIndicatorWidget->replace_textEditInfo_Display("info");
        return "error";
    }
    else if (function_select == "clear") {
        emotionIndicatorWidget->replace_textEditInfo_Display("");
    }
    return info;
}
