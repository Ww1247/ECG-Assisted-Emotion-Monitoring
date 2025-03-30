#ifndef SENSOR_DATA_H
#define SENSOR_DATA_H

#pragma once
#include <QString>
#include <QDateTime>
#include <QVariantMap>
#include <QImage>

struct SensorData {
    QString name;
    QVariantMap values;
    QImage image;
    QDateTime timestamp;
};

#endif // SENSOR_DATA_H
