#ifndef AHT20_H
#define AHT20_H

#include "i2c_driver.h"
#include <QObject>
#include <QThread>
#include <QDebug>

/**
 * @brief AHT20 sensor driver class.
 *
 * This class provides an interface for communicating with the AHT20 temperature
 * and humidity sensor using I2C protocol.
 */
class AHT20 : public QObject
{
    Q_OBJECT

public:

    /**
     * @brief Constructs an AHT20 sensor object.
     * @param i2cDriver A pointer to the I2CDriver instance handling communication.
     */
    explicit AHT20(I2CDriver *i2cDriver, QObject *parent = nullptr);

    /**
     * @brief Initializes the AHT20 sensor.
     * @return true if initialization is successful, false otherwise.
     */
    bool initialize();

    /**
     * @brief Triggers a measurement and reads temperature and humidity data.
     * @param temperature Reference to a float where temperature (�C) will be stored.
     * @param humidity Reference to a float where humidity (%) will be stored.
     * @return true if the measurement is successful, false otherwise.
     */
    bool readTemperatureAndHumidity(float &temperature, float &humidity);

private:

    I2CDriver *i2cDriver_; ///< Pointer to I2C driver instance.

    /**
     * @brief Sends a soft reset command to the AHT20 sensor.
     * @return true if successful, false otherwise.
     */
    bool softReset();

    /**
     * @brief Checks if the sensor is calibrated.
     * @return true if calibrated, false otherwise.
     */
    bool isCalibrated();

    /**
     * @brief Starts a measurement process.
     * @return true if successful, false otherwise.
     */
    bool triggerMeasurement();

signals:

    void errorOccurred(const QString &error_message);
};

#endif // AHT20_H
