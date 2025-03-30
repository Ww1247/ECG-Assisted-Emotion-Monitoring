#ifndef MAX30102_H
#define MAX30102_H

#pragma once
#include <QObject>
#include "sensor_interface.h"
#include "sensor_data.h"
#include "i2c_driver.h"

#define MAX30102_I2C_ADDRESS 0x57

class MAX30102 : public QObject, public SensorInterface
{
    Q_OBJECT

public:
    explicit MAX30102(I2CDriver *i2cDriver, QObject *parent = nullptr);
    ~MAX30102();

    bool initialize() override;
    bool readOnce(SensorData &data) override;
    void applySetting(const QVariantMap &params) override;

    void setMode(quint8 mode);  // Set working mode (HR, SPO2, Multi-LED)
    void setSamplingRate(quint8 rate);  // Adjust sample rate
    void setLEDPulseAmplitude(quint8 amplitude);  // Adjust LED brightness

private:
    I2CDriver *i2cDriver_;

};

#endif // MAX30102_H
