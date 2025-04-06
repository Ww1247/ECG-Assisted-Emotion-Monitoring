#ifndef MAX30102_H
#define MAX30102_H

#pragma once
#include <QObject>
#include "sensor_interface.h"
#include "sensor_data.h"
#include "i2c_driver.h"

/// I2C address of the MAX30102 sensor.
#define MAX30102_I2C_ADDRESS 0x57

/**
 * @brief Driver class for the MAX30102 heart rate and SpO2 sensor.
 *
 * This class provides methods to initialize the sensor, read measurement data,
 * and configure operating parameters like mode, sampling rate, and LED pulse amplitude.
 * It implements the SensorInterface for integration with the sensor management framework.
 */
class MAX30102 : public QObject, public SensorInterface
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a MAX30102 sensor object.
     * @param i2cDriver Pointer to the I2C driver for hardware communication.
     * @param parent Parent QObject.
     */
    explicit MAX30102(I2CDriver *i2cDriver, QObject *parent = nullptr);

    /**
     * @brief Destructor.
     */
    ~MAX30102();

    /**
     * @brief Initializes the MAX30102 sensor.
     * @return true if initialization is successful, false otherwise.
     */
    bool initialize() override;

    /**
     * @brief Reads heart rate and SpO2 data from the sensor.
     * @param data Output structure to hold the measurement results.
     * @return true if data was successfully read, false otherwise.
     */
    bool readOnce(SensorData &data) override;

    /**
     * @brief Applies configuration settings to the sensor.
     * @param params Key-value pairs of parameters (e.g., sampling rate, LED mode).
     */
    void applySetting(const QVariantMap &params) override;

    /**
     * @brief Sets the operating mode of the sensor.
     * @param mode 0 = HR only, 1 = SpO2, 2 = Multi-LED mode.
     */
    void setMode(quint8 mode);

    /**
     * @brief Sets the sampling rate for data acquisition.
     * @param rate Sampling rate in Hz.
     */
    void setSamplingRate(quint8 rate);

    /**
     * @brief Sets the LED pulse amplitude (brightness).
     * @param amplitude Pulse amplitude value (0�C255).
     */
    void setLEDPulseAmplitude(quint8 amplitude);

private:
    I2CDriver *i2cDriver_; ///< Driver for I2C communication with the sensor.
};

#endif // MAX30102_H
