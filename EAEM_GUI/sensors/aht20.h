#ifndef AHT20_H
#define AHT20_H

#pragma once
#include <QObject>
#include "sensor_interface.h"
#include "i2c_driver.h"
#include "sensor_data.h"

#define AHT20_I2C_ADDRESS     0x38  ///< I2C address of AHT20 sensor.
#define AHT20_STATUS_REG      0x71  ///< Status register (dummy read).
#define AHT20_CMD_TRIGGER     0xAC  ///< Trigger measurement command.
#define AHT20_CMD_SOFT_RESET  0xBA  ///< Soft reset command.
#define AHT20_CALIBRATION_BIT 0x08  ///< Calibration bit mask.

/**
 * @brief AHT20 sensor driver class.
 *
 * This class runs in a separate thread and continuously monitors the sensor's
 * status register. When new data is available, it triggers a measurement
 * and emits a signal with the updated temperature and humidity values.
 */
class AHT20 : public QObject, public SensorInterface
{
    Q_OBJECT

public:
    /**
     * @brief Constructs an AHT20 sensor object.
     * @param i2cDriver A pointer to the I2C driver for communication.
     */
    explicit AHT20(I2CDriver *i2cDriver, QObject *parent = nullptr);
    ~AHT20();

    /**
     * @brief Initializes the AHT20 sensor.
     * @return true if initialization is successful, false otherwise.
     */
    bool initialize() override;

    bool readOnce(SensorData &data) override;

    void applySetting(const QVariantMap &params) override;


private:
    I2CDriver *i2cDriver_;  ///< I2C communication driver.

    bool checkSensorReady();
};

#endif // AHT20_H
