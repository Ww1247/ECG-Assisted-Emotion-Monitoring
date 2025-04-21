#ifndef SENSORINTERFACE_H
#define SENSORINTERFACE_H

#pragma once
#include "sensor_data.h"

/**
 * @brief Abstract interface class for all sensor drivers.
 *
 * This interface standardizes how sensors are initialized, read, and configured
 * within the system. All sensor classes must implement this interface in order to
 * be registered and managed by the SensorManager.
 */
class SensorInterface {
public:
    /**
     * @brief Virtual destructor.
     */
    virtual ~SensorInterface() = default;

    /**
     * @brief Initializes the sensor.
     *
     * This may include hardware reset, I2C/SPI setup, calibration, etc.
     * @return true if initialization was successful, false otherwise.
     */
    virtual bool initialize() = 0;

    /**
     * @brief Reads a single frame or set of sensor data.
     *
     * The output is stored in the provided SensorData structure.
     * @param data Reference to the data structure to populate.
     * @return true if data was read successfully, false otherwise.
     */
    virtual bool readOnce(SensorData &data) = 0;

    /**
     * @brief Applies user-defined or system configuration to the sensor.
     *
     * This may include sampling rate, gain, resolution, mode, etc.
     * @param params A QVariantMap containing key-value configuration pairs.
     */
    virtual void applySetting(const QVariantMap &params) = 0;
};

#endif // SENSORINTERFACE_H
