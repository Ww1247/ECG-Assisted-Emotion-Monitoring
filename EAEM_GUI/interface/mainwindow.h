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

#define CONFIG_AHT20_SENSOR_FUNCTION_ENABLE 0
#define CONFIG_TEM_HUMID_FUNCTION_ENABLE    0

/**
 * @brief The MainWindow class serves as the main application window integrating all sensor components,
 * dashboard controls, emotion indicators, and data plotting widgets.
 *
 * It manages the UI setup, signal-slot connections, GPIO initialization, and sensor read lifecycle.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Constructs the main window and initializes all components.
     * @param parent The parent QWidget, default is nullptr.
     */
    MainWindow(QWidget *parent = nullptr);

    /**
     * @brief Destructor to clean up resources.
     */
    ~MainWindow();

public slots:
    /**
     * @brief Slot to handle errors emitted from various modules.
     * @param error_type Type or source of the error.
     * @param error_message Error message string.
     */
    void errorOccurred(const QString &error_type, const QString &error_message);

    /**
     * @brief Slot called when the "Start Emotion Detection" button is clicked.
     */
    void on_pushbutton_emotion_detection_start_clicked();

    /**
     * @brief Slot called when the "Stop Emotion Detection" button is clicked.
     */
    void on_pushbutton_emotion_detection_stop_clicked();

private:
    /**
     * @brief Initializes and arranges the UI layout and widgets.
     */
    void UI_SetUp();

    /**
     * @brief Initializes all inter-object signal and slot connections.
     */
    void initialize_SignalConnection();

    /**
     * @brief Initializes GPIO if available. Returns true on success.
     */
    bool initialize_GPIO();

    /**
     * @brief Starts GPIO initialization in a separate thread (non-blocking).
     */
    void run_GPIO_Initialize();

    /**
     * @brief Starts reading data from registered sensors.
     */
    void start_Sensor_Read();

    /**
     * @brief Stops reading data from sensors.
     */
    void stop_Sensor_Read();

    /**
     * @brief Utility function to format display messages.
     * @param function_selecte Function or module name.
     * @param info Message to be displayed.
     * @return Formatted string.
     */
    QString display_info(const QString &function_selecte, const QString &info);

    // Widgets
    DashboardWidget *dashboardWidget;                   ///< Control panel widget.
    VideoDisplayWidget *videoDisplayWidget;             ///< Live video feed display widget.
    EmotionIndicatorWidget *emotionIndicatorWidget;     ///< Widget for showing emotion detection results.
    SensorMAX30102Widget *sensorMAX30102Widget;         ///< MAX30102 sensor display widget.
    SensorAHT20Widget *sensorAHT20Widget;               ///< AHT20 sensor display widget.
    EcgHrvWidget *ecgHrvWidget;                         ///< ECG and HRV visualization widget.
    TemperatureHumidityWidget *temperatureHumidityWidget; ///< Temperature and humidity chart widget.

    SensorManager *manager = nullptr;                   ///< Sensor manager instance for unified control.
    PlotRefreshManager *plotManager;                    ///< Timer-driven plot refresher.

signals:
    /**
     * @brief Signal emitted with AHT20 sensor data.
     * @param temperature Temperature in degrees Celsius.
     * @param humidity Relative humidity percentage.
     */
    void sig_AHT20DataSend(const float &temperature, const float &humidity);

    /**
     * @brief Signal emitted with MAX30102 heart rate and SpO2 data.
     * @param heartrate Heart rate in bpm.
     * @param spo2 Blood oxygen saturation (%).
     */
    void sig_MAX30102DataSend(const float &heartrate, const float &spo2);

    /**
     * @brief Signal emitted with a camera frame image.
     * @param image Captured camera frame.
     */
    void sig_CameraDataSend(const QImage &image);

    /**
     * @brief Signal emitted with detected emotion and confidence.
     * @param emotion The predicted emotion label.
     * @param confidence Confidence score as a string.
     */
    void sig_sendEmotionandConfidence(const QString &emotion, const QString &confidence);
};
#endif // MAINWINDOW_H
