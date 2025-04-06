#ifndef SENSOR_AHT20_H
#define SENSOR_AHT20_H

#include <QWidget>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QSpacerItem>

/**
 * @brief The SensorAHT20Widget class provides a simple UI for displaying temperature and humidity readings.
 *
 * This widget is typically used to show live data from the AHT20 sensor.
 * It consists of two read-only line edits that display the latest values.
 */
class SensorAHT20Widget : public QWidget {

    Q_OBJECT

public:
    /**
     * @brief Constructs the AHT20 sensor widget.
     * @param parent The parent QWidget.
     */
    explicit SensorAHT20Widget(QWidget *parent = nullptr);

    /**
     * @brief Destructor for SensorAHT20Widget.
     */
    ~SensorAHT20Widget();

public slots:
    /**
     * @brief Updates the temperature and humidity values displayed on the UI.
     * @param temperature The current temperature value in degrees Celsius.
     * @param humidity The current relative humidity in percentage.
     */
    void updateValues(const float &temperature, const float &humidity);

private:
    /**
     * @brief Initializes and lays out the user interface components.
     */
    void initUI();

    QLineEdit *lineEdit_temperature_value; ///< Displays current temperature value.
    QLineEdit *lineEdit_humidity_value;    ///< Displays current humidity value.
};

#endif // SENSOR_AHT20_H
