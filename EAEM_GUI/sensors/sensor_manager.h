#ifndef SENSORMANAGER_H
#define SENSORMANAGER_H

#pragma once
#include <QObject>
#include <QMap>
#include <QThreadPool>
#include <QSharedPointer>
#include <atomic>
#include "sensor_data.h"
#include "sensor_task.h"
#include "sensor_interface.h"

/**
 * @brief Centralized manager class for handling multiple sensors using a thread pool.
 *
 * SensorManager allows registration, initialization, parallel task execution,
 * control command dispatch, and real-time data routing for all sensors in the system.
 */
class SensorManager : public QObject {
    Q_OBJECT

public:
    /**
     * @brief Constructs a SensorManager instance.
     * @param parent Parent QObject.
     */
    explicit SensorManager(QObject *parent = nullptr);

    /**
     * @brief Destructor. Ensures all sensors are stopped.
     */
    ~SensorManager();

    /**
     * @brief Registers a sensor with a unique name.
     * @param name Sensor name used for lookup.
     * @param sensor Pointer to the sensor implementation.
     */
    void registerSensor(const QString &name, SensorInterface *sensor);

    /**
     * @brief Starts reading data from the specified sensor using a background thread.
     * @param name Sensor name.
     */
    void startSensor(const QString &name);

    /**
     * @brief Stops reading data from the specified sensor.
     * @param name Sensor name.
     */
    void stopSensor(const QString &name);

    /**
     * @brief Stops all active sensors.
     */
    void stopAll();

    /**
     * @brief Checks whether a given sensor is currently active.
     * @param name Sensor name.
     * @return true if the sensor is running, false otherwise.
     */
    bool isRunning(const QString &name) const;

    /**
     * @brief Sends configuration or control commands to a sensor.
     * @param name Sensor name.
     * @param params Key-value map of settings.
     */
    void sendControlCommand(const QString &name, const QVariantMap &params);

    /**
     * @brief Forwards external data (e.g. from asynchronous processing) into the unified data flow.
     * @param data The sensor data to forward.
     */
    void forwardData(const SensorData &data);

signals:
    /**
     * @brief Emitted when new data is available from any sensor.
     * @param data The sensor data.
     */
    void dataReady(const SensorData &data);

    /**
     * @brief Emitted when a sensor reports an error.
     * @param name Sensor name.
     * @param msg Error message.
     */
    void errorOccurred(QString name, const QString &msg);

private:
    /**
     * @brief Internal structure representing a registered sensor and its status.
     */
    struct SensorEntry {
        SensorInterface *sensor;     ///< Pointer to the sensor instance.
        std::atomic_bool stopFlag;   ///< Flag to signal task cancellation.
        SensorEntry(SensorInterface *s)
            : sensor(s), stopFlag(false) {}
    };

    QMap<QString, QSharedPointer<SensorEntry>> sensors_;  ///< Registered sensor map.
};

#endif // SENSORMANAGER_H
