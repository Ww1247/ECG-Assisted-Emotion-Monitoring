#include "i2c_driver.h"
#include <QDebug>

QMutex I2CDriver::i2cMutex_;

I2CDriver::I2CDriver(int deviceAddress, QObject *parent)
    : QObject(parent),
      deviceAddress_(deviceAddress),
      i2cHandle_(-1)
{
//    initialize();
}

I2CDriver::~I2CDriver()
{
    if (i2cHandle_ >= 0) {
        i2cClose(i2cHandle_);
        qDebug() << "[INFO]: I2C Device Closed";
    }
}

bool I2CDriver::initialize()
{
    // Ensure pigpio is initialized externally.
    QMutexLocker locker(&i2cMutex_);
    // Open the I2C bus on bus 1 with the specified device address.
    i2cHandle_ = i2cOpen(1, deviceAddress_, 0);
    if (i2cHandle_ < 0) {
        qDebug() << "[ERROR]: Failed to open I2C device:" << intToHex(deviceAddress_);
        emit errorOccurred("Failed to Open I2C Device" + intToHex(deviceAddress_));
        return false;
    }
    qDebug() << "[LOG]: I2C Device Opened:" << intToHex(deviceAddress_);
    return true;
}

bool I2CDriver::close()
{
    QMutexLocker locker(&i2cMutex_);
    if (i2cHandle_ >= 0) {
        i2cClose(i2cHandle_);
        i2cHandle_ = -1;
        qDebug() << "[LOG]: I2C driver closed successfully";
        return true;
    }
    qDebug() << "[WARNING]: I2C driver already closed or not initialized";
    return false;
}

bool I2CDriver::writeByte(quint8 reg, quint8 value)
{
    QMutexLocker locker(&i2cMutex_);
    int result = i2cWriteByteData(i2cHandle_, reg, value);
    if (result < 0) {
        // qDebug() << "[LOG]: Written byte to register" << intToHex(reg) << ", Value:" << intToHex(value);
        emit errorOccurred("Failed to write byte to I2C device at register " + intToHex(reg));
        return false;
    }
    return true;
}

bool I2CDriver::writeBytes(quint8 reg, const QByteArray &values)
{
    QMutexLocker locker(&i2cMutex_);
    int result = i2cWriteI2CBlockData(i2cHandle_, reg, const_cast<char*>(values.data()), values.size());
    if (result < 0) {
        // qDebug() << "[LOG]: Written bytes to register" << intToHex(reg) << ", Value:" << values.size() << "bytes.";
        emit errorOccurred("Failed to write bytes to I2C device at register " + intToHex(reg));
        return false;
    }
    return true;
}

bool I2CDriver::readByte(quint8 reg, quint8 &value)
{
    QMutexLocker locker(&i2cMutex_);
    int result = i2cReadByteData(i2cHandle_, reg);
    if (result < 0) {
        value = static_cast<quint8>(result);
        // qDebug() << "[LOG]: Read byte from register" << intToHex(reg) << ", Value:" << intToHex(result);
        emit errorOccurred("Failed to read byte from I2C device at register " + intToHex(reg));
        return false;
    }
    return true;
}

QByteArray I2CDriver::readBytes(quint8 reg, quint8 count)
{
    QByteArray data(count, 0);  // Automating Memory Management with Qt Containers
    {
        QMutexLocker locker(&i2cMutex_);
        if (i2cReadI2CBlockData(i2cHandle_, reg, data.data(), count) != count) {
            // qDebug() << "[LOG]: Read bytes from register" << intToHex(reg) << ", Value:" << count << "bytes.";
            emit errorOccurred("Failed to read bytes from I2C device at register " + intToHex(reg));
            return QByteArray();
        }
        return data;
    }
}

QString I2CDriver::intToHex(int value)
{
    return QString("0x%1").arg(value & 0xFF, 2, 16, QChar('0')).toUpper();
}
