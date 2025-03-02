#include "mainwindow.h"
#include <QDebug>
#include "dashboard.h"
#include "video_display.h"
#include "emotion_indicator.h"
#include "sensor_max30102.h"
#include "sensor_aht20.h"
#include "ecg_hrv.h"
#include "temperature_humidity.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Emotion Detection System");
    resize(980, 1044);

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // Dashboard Module
    DashboardWidget *dashboard = new DashboardWidget(this);
    mainLayout->addWidget(dashboard,0);

    // Main Control Layout
    QHBoxLayout *horizontalLayout_Controler = new QHBoxLayout;

    // Video Display Module
    VideoDisplayWidget *videoDisplay = new VideoDisplayWidget(this);
    horizontalLayout_Controler->addWidget(videoDisplay,1);

    // Right Side Layout for Emotion Indicator and Sensors
    QVBoxLayout *verticalLayout_Sensors = new QVBoxLayout;

    // Emotion Indicator Module
    EmotionIndicatorWidget *emotionIndicator = new EmotionIndicatorWidget(this);
    verticalLayout_Sensors->addWidget(emotionIndicator,0);

    // MAX30102 Sensor Module
    SensorMAX30102Widget *sensorMAX30102 = new SensorMAX30102Widget(this);
    verticalLayout_Sensors->addWidget(sensorMAX30102,1);

    // AHT20 Sensor Module
    SensorAHT20Widget *sensorAHT20 = new SensorAHT20Widget(this);
    verticalLayout_Sensors->addWidget(sensorAHT20,1);

    horizontalLayout_Controler->addLayout(verticalLayout_Sensors,1);
    mainLayout->addLayout(horizontalLayout_Controler,0);

    // ECG & HRV Module
    EcgHrvWidget *ecg_hrv = new EcgHrvWidget(this);
    mainLayout->addWidget(ecg_hrv,1);

    // Temperature & Humidity Module
    TemperatureHumidityWidget *temperatureHumidity = new TemperatureHumidityWidget(this);
    mainLayout->addWidget(temperatureHumidity,1);

    // Set Layout and Central Widget
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);
    setStatusBar(new QStatusBar(this));
}

MainWindow::~MainWindow()
{
    // No need to delete UI here, as it is handled by Qt's parent-child memory management
    qDebug() << "Windows Closed.";
}
