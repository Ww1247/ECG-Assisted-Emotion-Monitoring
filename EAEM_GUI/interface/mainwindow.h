#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QVBoxLayout>
#include <QStatusBar>
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

    void on_pushbutton_emotion_detection_start_clicked();
    void on_pushbutton_emotion_detection_stop_clicked();

public slots:


private:
    void UI_SetUp();
    DashboardWidget *dashboardWidget;
    VideoDisplayWidget *videoDisplayWidget;
    EmotionIndicatorWidget *emotionIndicatorWidget;
    SensorMAX30102Widget *sensorMAX30102Widget;
    SensorAHT20Widget *sensorAHT20Widget;
    EcgHrvWidget *ecgHrvWidget;
    TemperatureHumidityWidget *temperatureHumidityWidget;

private slots:

};

#endif // MAINWINDOW_H
