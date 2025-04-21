#include "max30102.h"
#include <QThread>
#include <QDebug>

MAX30102::MAX30102(I2CDriver *i2cDriver, QObject *parent)
    : QObject(parent),
      i2cDriver_(i2cDriver)
{
    if (!i2cDriver_) {
        qDebug() << "Failed to initialize I2CDriver for MAX30102";
    }
}

MAX30102::~MAX30102()
{
//    i2cDriver->close(); // Ensure the I2C interface is properly closed on deletion
    qDebug() << "[LOG]: MAX30102 Distructed";
}

bool MAX30102::initialize()
{
    // Reset the device
    if (!i2cDriver_->writeByte(0x08, 0x40)) {
        qDebug() << "[MAX30102]: Failed to reset the device.";
        return false;
    }

    // Wait for the reset to complete
    quint8 resetComplete = 0x40;
    int attempts = 0;
    while ((resetComplete & 0x40) && attempts++ < 10) {
        QThread::msleep(10);
        if (!i2cDriver_->readByte(0x08, resetComplete)) {
            qDebug() << "[MAX30102]: Failed to read reset status.";
            return false;
        }
    }

    // Set to SPO2 mode (default)
    if (!i2cDriver_->writeByte(0x09, 0x03)) {
        qDebug() << "[MAX30102]: Failed to set SPO2 mode.";
        return false;
    }

    // Configure SPO2 sample rate, LED pulse width, ADC range
    if (!i2cDriver_->writeByte(0x0A, 0x27)) { // 100Hz, 411us
        qDebug() << "[MAX30102]: Failed to configure SPO2.";
        return false;
    }

    // Set LED pulse amplitude
    if (!i2cDriver_->writeByte(0x0C, 0x24)) { // RED LED
        qDebug() << "[MAX30102]: Failed to set RED LED amplitude.";
        return false;
    }
    if (!i2cDriver_->writeByte(0x0D, 0x24)) { // IR LED
        qDebug() << "[MAX30102]: Failed to set IR LED amplitude.";
        return false;
    }

    qDebug() << "[LOG]: MAX30102 initialized successfully.";
    return true;
}

bool MAX30102::readOnce(SensorData &data)
{
    QByteArray raw = i2cDriver_->readBytes(0x06, 6);  // FIFO_DATA register

    if (raw.size() != 6) {
        qDebug() << "[MAX30102]: Failed to read FIFO data.";
        return false;
    }

    // FIFO Data Format:
    // [0] IR(17~16), [1] IR(15~8), [2] IR(7~0)
    // [3] RED(17~16), [4] RED(15~8), [5] RED(7~0)

    quint32 irRaw = ((quint8)raw[0] << 16) | ((quint8)raw[1] << 8) | (quint8)raw[2];
    quint32 redRaw = ((quint8)raw[3] << 16) | ((quint8)raw[4] << 8) | (quint8)raw[5];

    irRaw &= 0x3FFFF;   // 18-bit mask
    redRaw &= 0x3FFFF;

    // Mock result
    float heartRate = 70.0f + (redRaw % 10);  // mock HR
    float spo2 = 95.0f + (irRaw % 3);         // mock SpO2

    // Fill SensorData
    data.sensor_name = "MAX30102";
    data.timestamp = QDateTime::currentDateTime();
    data.values["heartRate"] = heartRate;
    data.values["spo2"] = spo2;

    return true;
}

void MAX30102::applySetting(const QVariantMap &params)
{
    if (params.contains("mode")) {
        quint8 mode = params["mode"].toUInt();
        setMode(mode);
    }
    if (params.contains("samplingRate")) {
        quint8 rate = params["sampling rate"].toUInt();
        setSamplingRate(rate);
    }
    if (params.contains("ledAmplitude")) {
        quint8 amp = params["ledAmplitude"].toUInt();
        setLEDPulseAmplitude(amp);
    }
}

// Set working mode (0x02 = HR Mode, 0x03 = SPO2 Mode, 0x07 = Multi-LED Mode)
void MAX30102::setMode(quint8 mode) {
    if (!i2cDriver_->writeByte(0x09, mode)) {
        qDebug() << "[MAX30102]: Failed to set sensor mode.";
    }
}

// Adjust sampling rate (0x00 = 50Hz, 0x01 = 100Hz, 0x02 = 200Hz, etc.)
void MAX30102::setSamplingRate(quint8 rate) {
    if (!i2cDriver_->writeByte(0x0A, (rate & 0x07) << 2)) { // Mask lower 3 bits
        qDebug() << "[MAX30102]: Failed to set sampling rate.";
    }
}

// Adjust LED brightness (0x00 ~ 0xFF)
void MAX30102::setLEDPulseAmplitude(quint8 amplitude) {
    if (!i2cDriver_->writeByte(0x0C, amplitude)) {
        qDebug() << "[MAX30102]: Failed to set LED pulse amplitude.";
    }
}


