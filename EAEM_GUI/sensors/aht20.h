#ifndef AHT20_H
#define AHT20_H

#pragma once
#include <QObject>
#include "sensor_interface.h"
#include "i2c_driver.h"
#include "sensor_data.h"

/// I2C address of the AHT20 sensor.
#define AHT20_I2C_ADDRESS     0x38

/// Register definitions and command constants.
#define AHT20_STATUS_REG      0x71  ///< Status register (dummy read).
#define AHT20_CMD_TRIGGER     0xAC  ///< Trigger measurement command.
#define AHT20_CMD_SOFT_RESET  0xBA  ///< Soft reset command.
#define AHT20_CALIBRATION_BIT 0x08  ///< Calibration bit mask.

/**
 * @brief AHT20 sensor driver class.
 *
 * This class communicates with the AHT20 temperature and humidity sensor over I2C,
 * implementing the SensorInterface for integration with the SensorManager system.
 * It supports initialization, data reading, and optional configuration.
 */
class AHT20 : public QObject, public SensorInterface
{
    Q_OBJECT

public:
    /**
     * @brief Constructs an AHT20 sensor object.
     * @param i2cDriver A pointer to the I2C driver for communication.
     * @param parent The parent QObject.
     */
    explicit AHT20(I2CDriver *i2cDriver, QObject *parent = nullptr);

    /**
     * @brief Destructor.
     */
    ~AHT20();

    /**
     * @brief Initializes the AHT20 sensor.
     *
     * Typically involves a soft reset and checking calibration status.
     * @return true if the sensor was initialized successfully, false otherwise.
     */
    bool initialize() override;

    /**
     * @brief Reads a single temperature and humidity measurement from the sensor.
     * @param data Output structure to hold the read values.
     * @return true if data was read successfully, false on error.
     */
    bool readOnce(SensorData &data) override;

    /**
     * @brief Applies additional configuration to the sensor (not used for AHT20).
     * @param params Key-value map of configuration parameters.
     */
    void applySetting(const QVariantMap &params) override;

private:
    I2CDriver *i2cDriver_;  ///< I2C communication driver used to talk to the sensor.

    /**
     * @brief Checks whether the sensor is calibrated and ready to provide data.
     * @return true if sensor is ready, false otherwise.
     */
    bool checkSensorReady();
};

#endif // AHT20_H
