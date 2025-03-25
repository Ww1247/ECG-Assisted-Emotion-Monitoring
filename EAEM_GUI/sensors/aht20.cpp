#include "aht20.h"
#include <QThread>
#include <QDebug>

#define AHT20_I2C_ADDRESS     0x38  ///< I2C address of AHT20 sensor.
#define AHT20_STATUS_REG      0x71  ///< Status register (dummy read).
#define AHT20_CMD_TRIGGER     0xAC  ///< Trigger measurement command.
#define AHT20_CMD_SOFT_RESET  0xBA  ///< Soft reset command.
#define AHT20_CALIBRATION_BIT 0x08  ///< Calibration bit mask.

/**
 * @brief Constructs an AHT20 sensor object.
 * @param i2cDriver A pointer to the I2C driver for communication.
 */
AHT20::AHT20(I2CDriver *i2cDriver, QObject *parent)
    : QObject(parent),
      i2cDriver_(i2cDriver),
      keepRunning_(false)
{
    if (!i2cDriver_) {
        qDebug() << "Failed to initialize I2CDriver for AHT20";
        // emit error signal here
        return;
    }
}

AHT20::~AHT20()
{
    stopMonitoring();
}

/**
 * @brief Initializes the AHT20 sensor.
 * @return true if initialization is successful, false otherwise.
 */
bool AHT20::initialize()
{
    // Soft reset the sensor
    if (!i2cDriver_->writeByte(0x00, AHT20_CMD_SOFT_RESET)) {
        qDebug() << "AHT20 soft reset failed.";
        return false;
    }

    QThread::msleep(10); // Wait after reset

    // Read status register to check if the sensor is calibrated
    quint8 status;
    if (!i2cDriver_->readByte(AHT20_STATUS_REG, status)) {
        qDebug() << "Failed to read AHT20 status register.";
        return false;
    }

    if (!(status & AHT20_CALIBRATION_BIT)) {
        // Send initialization command
        QByteArray initCmd;
        initCmd.append(static_cast<quint8>(0xBE));
        initCmd.append(static_cast<quint8>(0x08));
        initCmd.append(static_cast<quint8>(0x00));

        if (!i2cDriver_->writeBytes(0x00, initCmd)) {
            qDebug() << "AHT20 initialization failed.";
            return false;
        }

        QThread::msleep(10); // Wait for initialization
    }

    qDebug() << "[LOG]: AHT20 initialized successfully.";
    return true;
}

/**
 * @brief Continuously monitors the sensor's status register.
 * When new data is available, it reads the sensor values.
 */
void AHT20::startMonitorSensor()
{
    qDebug() << "[LOG]: AHT20 reading start.";
    keepRunning_ = true;
    while (keepRunning_) {
        if (checkSensorReady()) {
            float temperature, humidity;

            // Trigger a measurement
            QByteArray cmd;
            cmd.append(static_cast<quint8>(AHT20_CMD_TRIGGER));
            cmd.append(static_cast<quint8>(0x33));
            cmd.append(static_cast<quint8>(0x00));
            if (!i2cDriver_->writeBytes(0x00, cmd)) {
                qDebug() << "Failed to send measurement command.";
                continue;
            }

            QThread::msleep(80);  // Wait for measurement to complete

            // Read measurement data
            QByteArray data = i2cDriver_->readBytes(0x00, 7);
            if (data.size() != 7) {
                qDebug() << "Failed to read measurement data.";
                continue;
            }

            // Parse raw humidity and temperature values
            quint32 rawHumidity = (quint32(data[1]) << 12) | (quint32(data[2]) << 4) | (quint32(data[3]) >> 4);
            quint32 rawTemperature = ((quint32(data[3]) & 0x0F) << 16) | (quint32(data[4]) << 8) | quint32(data[5]);

            humidity = (rawHumidity * 100.0) / 1048576.0;
            temperature = ((rawTemperature * 200.0) / 1048576.0) - 50.0;

            // Emit signal with new data
//            qDebug() << temperature << ", " << humidity;
            emit dataReady(temperature, humidity);
        }

        if (!keepRunning_) break; // Make sure that the thread end immediately
        QThread::msleep(500);  // Avoid excessive polling
    }
}

/**
 * @brief Stops the monitoring loop safely.
 */
void AHT20::stopMonitoring()
{
    keepRunning_.storeRelaxed(false);  // Stop the processing loop
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
