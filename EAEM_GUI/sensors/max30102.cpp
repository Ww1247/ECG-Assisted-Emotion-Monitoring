#include "max30102.h"
#include <QThread>
#include <QDebug>

MAX30102::MAX30102(int deviceAddress, QObject *parent)
    : QObject(parent),
      deviceAddress_(deviceAddress),
      keepRunning_(false)
{
    i2cDriver = new I2CDriver(deviceAddress_, this);
    qDebug() << "[LOG]: MAX30102 current thread: " << QThread::currentThreadId();
}

MAX30102::~MAX30102()
{
//    i2cDriver->close(); // Ensure the I2C interface is properly closed on deletion
    qDebug() << "[LOG]: MAX30102 Distructed";
}

void MAX30102::process()
{
    keepRunning_.storeRelaxed(true);  // Start the processing loop with relaxed memory ordering
    while (keepRunning_.loadRelaxed() != 0) {  // Use loadRelaxed for checking the flag
        if (QThread::currentThread()->isInterruptionRequested()) {
            keepRunning_.storeRelaxed(false);  // Stop the processing loop
        }
        QByteArray data = readSensorData();
        emit dataReady(data);
        QThread::msleep(100);  // Sleep to prevent tight looping
    }
    emit finished();
}

void MAX30102::stop()
{
    keepRunning_.storeRelaxed(false);  // Stop the processing loop
}

bool MAX30102::initialize()
{
    // Reset the device
    if (!i2cDriver->writeByte(0x08, 0x40)) {
        emit errorOccurred("Failed to reset the device.");
        return false;
    }

    // Wait for the reset to complete
    quint8 resetComplete;
    do {
        if (!i2cDriver->readByte(0x08, resetComplete)) {
            emit errorOccurred("Failed to read reset status.");
            return false;
        }
    } while (resetComplete & 0x40);

    // Set to SPO2 mode
    if (!i2cDriver->writeByte(0x09, 0x03)) {
        emit errorOccurred("Failed to set SPO2 mode.");
        return false;
    }

    // Configure SPO2 sample rate and LED pulse width
    if (!i2cDriver->writeByte(0x0A, 0x27)) { // 100Hz, 411us, MAX
        emit errorOccurred("Failed to configure SPO2.");
        return false;
    }

    // Set LED pulse amplitude
    if (!i2cDriver->writeByte(0x0C, 0x24)) { // LED1 and LED2
        emit errorOccurred("Failed to set LED pulse amplitude.");
        return false;
    }
    qDebug() << "[LOG]: MAX30102 initial successfully";
    return true;
}

QByteArray MAX30102::readSensorData() {
    return i2cDriver->readBytes(0x06, 6); // Read 6 bytes of FIFO data
}

// Set working mode (0x02 = HR Mode, 0x03 = SPO2 Mode, 0x07 = Multi-LED Mode)
void MAX30102::setMode(quint8 mode) {
    if (!i2cDriver->writeByte(0x09, mode)) {
        emit errorOccurred("Failed to set sensor mode.");
    }
}

// Adjust sampling rate (0x00 = 50Hz, 0x01 = 100Hz, 0x02 = 200Hz, etc.)
void MAX30102::setSamplingRate(quint8 rate) {
    if (!i2cDriver->writeByte(0x0A, (rate & 0x07) << 2)) { // Mask lower 3 bits
        emit errorOccurred("Failed to set sampling rate.");
    }
}

// Adjust LED brightness (0x00 ~ 0xFF)
void MAX30102::setLEDPulseAmplitude(quint8 amplitude) {
    if (!i2cDriver->writeByte(0x0C, amplitude)) {
        emit errorOccurred("Failed to set LED pulse amplitude.");
    }
}
