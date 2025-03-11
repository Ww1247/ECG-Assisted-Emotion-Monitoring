#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QVBoxLayout>
#include <QStatusBar>
#include <QThread>
#include <QList>
#include <opencv2/opencv.hpp>

#include "dashboard.h"
#include "video_display.h"
#include "emotion_indicator.h"
#include "sensor_max30102.h"
#include "sensor_aht20.h"
#include "ecg_hrv.h"
#include "temperature_humidity.h"


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
    bool initialize_GPIO();
    bool initialize_Camera();
    bool initialize_SensorReading();
    void run_GPIO_Initialize();
    void run_Camera_Initialize();
    void run_Sensor_Read();
    void display_info(const QString &function_selecte, const QString &info);

    DashboardWidget *dashboardWidget;
    VideoDisplayWidget *videoDisplayWidget;
    EmotionIndicatorWidget *emotionIndicatorWidget;
    SensorMAX30102Widget *sensorMAX30102Widget;
    SensorAHT20Widget *sensorAHT20Widget;
    EcgHrvWidget *ecgHrvWidget;
    TemperatureHumidityWidget *temperatureHumidityWidget;

private slots:

signals:
    void sig_videoCaptureStop();
    void sig_videoCaptureStart(int camera_index);

};

#endif // MAINWINDOW_H
