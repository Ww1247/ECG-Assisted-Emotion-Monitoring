#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QVBoxLayout>
#include <QStatusBar>
#include <QThread>
#include <QTimer>
#include <QList>
#include <opencv2/opencv.hpp>

#include "dashboard.h"
#include "video_display.h"
#include "emotion_indicator.h"
#include "sensor_max30102.h"
#include "sensor_aht20.h"
#include "ecg_hrv.h"
#include "temperature_humidity.h"
#include "sensor_manager.h"
#include "aht20.h"
#include "max30102.h"
#include "i2c_driver.h"
#include "plot_refresh_manager.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void errorOccurred(const QString &error_type, const QString &error_message);
    void on_pushbutton_emotion_detection_start_clicked();
    void on_pushbutton_emotion_detection_stop_clicked();

private:
    void UI_SetUp();
    void initialize_SignalConnection();
    bool initialize_GPIO();
    void run_GPIO_Initialize();
    void start_Sensor_Read();
    void stop_Sensor_Read();

    QString display_info(const QString &function_selecte, const QString &info);

    DashboardWidget *dashboardWidget;
    VideoDisplayWidget *videoDisplayWidget;
    EmotionIndicatorWidget *emotionIndicatorWidget;
    SensorMAX30102Widget *sensorMAX30102Widget;
    SensorAHT20Widget *sensorAHT20Widget;
    EcgHrvWidget *ecgHrvWidget;
    TemperatureHumidityWidget *temperatureHumidityWidget;
    SensorManager *manager = nullptr;
    PlotRefreshManager *plotManager;

signals:
    void sig_AHT20DataSend(const float &temperature, const float &humidity);
    void sig_MAX30102DataSend(const float &heartrate, const float &spo2);
    void sig_CameraDataSend(const QImage &image);

};

#endif // MAINWINDOW_H
