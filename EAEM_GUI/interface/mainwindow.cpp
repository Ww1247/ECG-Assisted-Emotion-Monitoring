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
    run_GPIO_Initialize();
}

MainWindow::~MainWindow()
{
    stop_Sensor_Read();

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

void MainWindow::initialize_SignalConnection()
{
    /*SensorManager Instantiation*/
    manager = new SensorManager(this);

    /*Register AHT20 Sensor*/
    I2CDriver *i2c_aht20 = new I2CDriver(AHT20_I2C_ADDRESS, this);
    if (!i2c_aht20->initialize()) {
        qDebug() << "[ERROR]: AHT20 I2C initialization failed.";
        delete i2c_aht20;
        return;
    }
    AHT20 *aht20 = new AHT20(i2c_aht20);
    manager->registerSensor("AHT20", aht20);

    /*Register MAX30102 Sensor*/
    I2CDriver *i2c_max30102 = new I2CDriver(MAX30102_I2C_ADDRESS, this);
    if (!i2c_max30102->initialize()) {
        qDebug() << "[ERROR]: MAX30102 I2C initialization failed.";
        delete i2c_max30102;
        return;
    }
    MAX30102 *max30102 = new MAX30102(i2c_max30102, this);
    manager->registerSensor("MAX30102", max30102);

    /*Register Camera Sensor*/
    CameraDriver *camera = new CameraDriver(0, 30, 640, 480, this);
    manager->registerSensor("Camera", camera);

    /*Register Plotting Refresh Manager*/
    plotManager = new PlotRefreshManager(this);
    plotManager->registerPlot(ecgHrvWidget);
    plotManager->registerPlot(temperatureHumidityWidget);

    /**/
    qAddPostRoutine([](){
        gpioTerminate();
        qDebug() << "[LOG]: gpioTerminate called at post-routine shutdown.";
    });

    /*Establish DashboardWidget Signal Connections*/
    connect(dashboardWidget, &DashboardWidget::sig_emotion_detection_start, this, &MainWindow::on_pushbutton_emotion_detection_start_clicked);
    connect(dashboardWidget, &DashboardWidget::sig_emotion_detection_stop, this, &MainWindow::on_pushbutton_emotion_detection_stop_clicked);
    connect(this, &MainWindow::sig_AHT20DataSend, sensorAHT20Widget, &SensorAHT20Widget::updateValues);
    connect(this, &MainWindow::sig_MAX30102DataSend, sensorMAX30102Widget, &SensorMAX30102Widget::updateValues);
    connect(this, &MainWindow::sig_CameraDataSend, videoDisplayWidget, &VideoDisplayWidget::updateFrame);
    connect(this, &MainWindow::sig_sendEmotionandConfidence, emotionIndicatorWidget, &EmotionIndicatorWidget::emotion_status_receiver);

    /* SensorManager Data Handling */
    connect(manager, &SensorManager::dataReady, this, [=](const SensorData &data) {
        if (data.sensor_name == "AHT20") {
            float t = data.values.value("temperature").toFloat();
            float h = data.values.value("humidity").toFloat();
            emit sig_AHT20DataSend(t, h);
            temperatureHumidityWidget->addData(t, h);
        } else if (data.sensor_name == "MAX30102") {
            float hr = data.values.value("heartRate").toFloat();
            float sp = data.values.value("spo2").toFloat();
            emit sig_MAX30102DataSend(hr, sp);
            ecgHrvWidget->addECGData(hr, sp);
        } else if (data.sensor_name == "Camera" && !data.image.isNull()) {
            emit sig_CameraDataSend(data.image);
            if (data.values.contains("emotion")) {
                QString emotion = data.values.value("emotion").toString();
                float confidence = data.values.value("confidence").toFloat();
                QString confidenceStr = QString::number(confidence, 'f', 3);
                emit sig_sendEmotionandConfidence(emotion, confidenceStr);
//                qDebug() << "[Emotion] Detected:" << emotion << "| Confidence:" << confidence;
            }
        }
    });
}

void MainWindow::on_pushbutton_emotion_detection_start_clicked()
{
    qDebug() << "[LOG]: System Start";
    start_Sensor_Read();
}

void MainWindow::start_Sensor_Read()
{
    qDebug() << "[LOG]: Turn On Sensor Reading ...";
    manager->startSensor("AHT20");
    manager->startSensor("MAX30102");
    manager->startSensor("Camera");
    qDebug() << "[LOG]: Sensor reading started.";
}

void MainWindow::on_pushbutton_emotion_detection_stop_clicked()
{
    display_info("append", "System Stop");
    stop_Sensor_Read();
}

void MainWindow::stop_Sensor_Read()
{
    manager->stopAll();
}

void MainWindow::run_GPIO_Initialize()
{
    QFutureWatcher<bool> *watcher = new QFutureWatcher<bool>(this);
    connect(watcher, &QFutureWatcher<bool>::finished, this, [=]() {
        bool success = watcher->future().result();
        if (success) {
            initialize_SignalConnection();
        }
        else {
            errorOccurred("GPIO ERROR", "Failed to initialize pigpio after 5 attempts.");
        }
        watcher->deleteLater();
    });
    QFuture<bool> future = QtConcurrent::run(this, &MainWindow::initialize_GPIO);
    watcher->setFuture(future);
}

bool MainWindow::initialize_GPIO()
{
    int retryCount = 0;
    const int maxRetries = 5;

    while (retryCount < maxRetries) {
        if (gpioInitialise() < 0) {
            QThread::sleep(1);
            retryCount++;
            qDebug() << "[ERROR]: Failed to initialize pigpio. Retrying " << retryCount << " attempts.";
            continue;
        }
        qDebug() << "[LOG]: Pigpio initialized successfully!";
        return true;
    }
    qDebug() << "[ERROR]: Failed to initialize pigpio after 5 attempts.";
    return false;
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
