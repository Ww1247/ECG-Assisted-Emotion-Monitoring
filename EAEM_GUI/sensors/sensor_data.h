#ifndef SENSOR_DATA_H
#define SENSOR_DATA_H

#pragma once
#include <QString>
#include <QDateTime>
#include <QVariantMap>
#include <QImage>

/**
 * @brief Struct representing a generic sensor data packet.
 *
 * This structure is used to encapsulate readings from any sensor,
 * including scalar values, images, timestamps, and sensor identifiers.
 */
struct SensorData {
    QString sensor_name;    ///< Name or identifier of the sensor that produced the data.
    QVariantMap values;     ///< Key-value map containing sensor readings (e.g., temperature, heart rate).
    QImage image;           ///< Optional image (used for camera or vision-based sensors).
    QDateTime timestamp;    ///< Timestamp of when the data was collected.
};

#endif // SENSOR_DATA_H
