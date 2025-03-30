#include "aht20.h"
#include <QThread>
#include <QDebug>

/**
 * @brief Constructs an AHT20 sensor object.
 * @param i2cDriver A pointer to the I2C driver for communication.
 */
AHT20::AHT20(I2CDriver *i2cDriver, QObject *parent)
    : QObject(parent),
      i2cDriver_(i2cDriver)
{
    if (!i2cDriver_) {
        qDebug() << "Failed to initialize I2CDriver for AHT20";
    }
}

AHT20::~AHT20()
{
    qDebug() << "[LOG]: AHT20 Distructed";
}


/**
 * @brief Initializes the AHT20 sensor.
 * @return true if initialization is successful, false otherwise.
 */
bool AHT20::initialize()
{
    // Soft reset the sensor
    if (!i2cDriver_->writeByte(0x00, AHT20_CMD_SOFT_RESET)) {
        qDebug() << "[AHT20]: soft reset failed.";
        return false;
    }

    QThread::msleep(10); // Wait after reset

    // Read status register to check if the sensor is calibrated
    quint8 status;
    if (!i2cDriver_->readByte(AHT20_STATUS_REG, status)) {
        qDebug() << "[AHT20]: Failed to read AHT20 status register.";
        return false;
    }

    if (!(status & AHT20_CALIBRATION_BIT)) {
        // Send initialization command
        QByteArray initCmd;
        initCmd.append(static_cast<quint8>(0xBE));
        initCmd.append(static_cast<quint8>(0x08));
        initCmd.append(static_cast<quint8>(0x00));

        if (!i2cDriver_->writeBytes(0x00, initCmd)) {
            qDebug() << "[AHT20]: initialization failed.";
            return false;
        }

        QThread::msleep(10); // Wait for initialization
    }

    qDebug() << "[LOG]: AHT20 initialized successfully.";
    return true;
}


bool AHT20::readOnce(SensorData &data)
{
    if (!checkSensorReady()) {
        qDebug() << "[AHT20] Sensor not ready.";
        return false;
    }

    // Send trigger measurement command
    QByteArray cmd;
    cmd.append(static_cast<quint8>(AHT20_CMD_TRIGGER));
    cmd.append(static_cast<quint8>(0x33));
    cmd.append(static_cast<quint8>(0x00));
    if (!i2cDriver_->writeBytes(0x00, cmd)) {
        qDebug() << "[AHT20] Write measurement command failed.";
        return false;
    }

    QThread::msleep(80);  // Wait for measurement

    // Read 7 bytes of data
    QByteArray raw = i2cDriver_->readBytes(0x00, 7);
    if (raw.size() != 7) {
        qDebug() << "[AHT20] Read data failed.";
        return false;
    }

    // Parse raw data
    quint32 rawHumidity = (quint32(raw[1]) << 12) | (quint32(raw[2]) << 4) | (quint32(raw[3]) >> 4);
    quint32 rawTemperature = ((quint32(raw[3]) & 0x0F) << 16) | (quint32(raw[4]) << 8) | quint32(raw[5]);

    float humidity = (rawHumidity * 100.0f) / 1048576.0f;
    float temperature = ((rawTemperature * 200.0f) / 1048576.0f) - 50.0f;

    // Fill SensorData
    data.name = "AHT20";
    data.timestamp = QDateTime::currentDateTime();
    data.values["temperature"] = temperature;
    data.values["humidity"] = humidity;

//    qDebug() << "[AHT20] Read success. T:" << temperature << "H:" << humidity;
    return true;
}

/**
 * @brief Checks if the AHT20 sensor is ready to provide new data.
 * @return true if data is available, false otherwise.
 */
bool AHT20::checkSensorReady()
{
    quint8 status;
    if (i2cDriver_->readByte(AHT20_STATUS_REG, status)) {
        return (status & 0x80) == 0;  // Bit 7 set to 0 means data is ready
    }
    return false;
}

void AHT20::applySetting(const QVariantMap &params)
{

}
