#ifndef SENSOR_MAX30102_H
#define SENSOR_MAX30102_H

#include <QWidget>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QSlider>
#include <QSpacerItem>
#include <QComboBox>

/**
 * @brief The SensorMAX30102Widget class provides a graphical interface for displaying
 * and controlling the MAX30102 heart rate and SpO? sensor.
 *
 * It includes value displays, control elements for LED configuration and sampling rate,
 * and UI elements for different operating modes.
 */
class SensorMAX30102Widget : public QWidget {

    Q_OBJECT

public:
    /**
     * @brief Constructs the SensorMAX30102Widget UI component.
     * @param parent Parent widget.
     */
    explicit SensorMAX30102Widget(QWidget *parent = nullptr);

    /**
     * @brief Destructor.
     */
    ~SensorMAX30102Widget();

private:
    /**
     * @brief Initializes and lays out all UI components.
     */
    void initUI();

    // Combo boxes for sensor configuration
    QComboBox *comboBox_sampling_rate;   ///< Dropdown for selecting sampling rate.
    QComboBox *comboBox_led_red;         ///< Dropdown for selecting RED LED current.
    QComboBox *comboBox_led_ir;          ///< Dropdown for selecting IR LED current.

    // Checkboxes for mode selection
    QCheckBox *checkBox_heart_rate_mode;     ///< Checkbox for heart rate only mode.
    QCheckBox *checkBox_blood_oxygen_mode;   ///< Checkbox for SpO? only mode.
    QCheckBox *checkBox_mixed_mode;          ///< Checkbox for mixed measurement mode.

    // Sliders for real-time value adjustment
    QSlider *slider_heart_rate;              ///< Slider for simulated heart rate display.
    QSlider *slider_blood_oxygen;            ///< Slider for simulated SpO? display.

    // Display fields
    QLineEdit *lineEdit_heart_rate_value;    ///< Display field for heart rate value.
    QLineEdit *lineEdit_blood_oxygen_value;  ///< Display field for SpO? value.

public slots:
    /**
     * @brief Updates the displayed heart rate and SpO? values.
     * @param heartrate The heart rate value in bpm.
     * @param spo2 The blood oxygen saturation in percentage.
     */
    void updateValues(const float &heartrate, const float &spo2);

private slots:
    /**
     * @brief Handles changes to the sampling rate dropdown.
     * @param index The selected index.
     */
    void on_comboBox_sampling_rate_currentIndexChanged(int index);

    /**
     * @brief Handles changes to the RED LED configuration.
     * @param index The selected index.
     */
    void on_comboBox_led_red_currentIndexChanged(int index);

    /**
     * @brief Handles changes to the IR LED configuration.
     * @param index The selected index.
     */
    void on_comboBox_led_ir_currentIndexChanged(int index);

    /**
     * @brief Handles toggle state changes for mode checkboxes.
     * @param checked Checkbox state.
     */
    void on_checkBox_mode_changed(bool checked);

    /**
     * @brief Updates heart rate value based on slider.
     * @param value Slider value.
     */
    void on_slider_heart_rate_changed(int value);

    /**
     * @brief Updates SpO? value based on slider.
     * @param value Slider value.
     */
    void on_slider_blood_oxygen_changed(int value);

signals:
    /**
     * @brief Signal emitted to request a configuration change to the sensor.
     * @param name Name of the sensor.
     * @param params Key-value control parameters.
     */
    void sig_SendControl(const QString &name, const QVariantMap &params);
};

#endif // SENSOR_MAX30102_H
