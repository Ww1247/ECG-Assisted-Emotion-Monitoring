#ifndef MAX30102_H
#define MAX30102_H

#include <QObject>
#include <QAtomicInt>

#include "i2c_driver.h"

class MAX30102 : public QObject
{
    Q_OBJECT
public:
    explicit MAX30102(int deviceAddress, QObject *parent = nullptr);
    ~MAX30102();

public slots:
    void process();
    void stop();

    bool initialize();
    void setMode(quint8 mode);  // Set working mode (HR, SPO2, Multi-LED)
    void setSamplingRate(quint8 rate);  // Adjust sample rate
    void setLEDPulseAmplitude(quint8 amplitude);  // Adjust LED brightness

private:
    I2CDriver *i2cDriver;
    QAtomicInt keepRunning_;
    int deviceAddress_;

    QByteArray readSensorData();

signals:
    void dataReady(const QByteArray &data);
    void errorOccurred(const QString &error);
    void finished();

};

#endif // MAX30102_H
