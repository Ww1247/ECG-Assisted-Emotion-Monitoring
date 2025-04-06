#ifndef SENSORTASK_H
#define SENSORTASK_H

#pragma once
#include <QObject>
#include <QRunnable>
#include <QPointer>
#include <atomic>
#include "sensor_interface.h"
#include "sensor_data.h"

/**
 * @brief The SensorTask class represents a threaded task that reads data from a sensor.
 *
 * It implements QRunnable to run in a QThreadPool, and continuously collects data from a given sensor
 * until the provided stop flag is set. The results are emitted via Qt signals for downstream processing.
 */
class SensorTask : public QObject, public QRunnable {
    Q_OBJECT

public:
    /**
     * @brief Constructs a SensorTask.
     * @param name The name of the sensor (used in logs and error reporting).
     * @param sensor Pointer to a sensor object implementing SensorInterface.
     * @param stopFlag Pointer to an atomic flag used to signal task termination.
     */
    SensorTask(const QString &name, SensorInterface *sensor, std::atomic_bool *stopFlag);

    /**
     * @brief The task's main execution loop, runs inside a thread.
     * Continuously reads sensor data until stopFlag is set.
     */
    void run() override;

signals:
    /**
     * @brief Emitted when new sensor data is successfully retrieved.
     * @param data The data structure containing readings.
     */
    void dataReady(const SensorData &data);

    /**
     * @brief Emitted when an error occurs during sensor reading.
     * @param name Name of the sensor.
     * @param errorMsg Error message.
     */
    void errorOccurred(QString name, const QString &errorMsg);

private:
    QString name_;                     ///< Sensor name for identification.
    SensorInterface *sensor_;          ///< Pointer to the sensor instance.
    std::atomic_bool *stopFlag_;       ///< External flag to control task execution.
};

#endif // SENSORTASK_H
