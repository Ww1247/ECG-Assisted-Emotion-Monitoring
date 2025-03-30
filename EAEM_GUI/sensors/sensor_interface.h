#ifndef SENSORINTERFACE_H
#define SENSORINTERFACE_H

#pragma once
#include "sensor_data.h"

class SensorInterface {
public:
    virtual ~SensorInterface() = default;
    virtual bool initialize() = 0;
    virtual bool readOnce(SensorData &data) = 0;
    virtual void applySetting(const QVariantMap &params) = 0;
};

#endif // SENSORINTERFACE_H
