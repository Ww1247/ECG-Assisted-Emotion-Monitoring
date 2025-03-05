#ifndef AHT20_H
#define AHT20_H

#include <QObject>
#include <QAtomicInt>

#include "i2c_driver.h"

/**
 * @brief AHT20 sensor driver class.
 *
 * This class runs in a separate thread and continuously monitors the sensor's
 * status register. When new data is available, it triggers a measurement
 * and emits a signal with the updated temperature and humidity values.
 */
class AHT20 : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Constructs an AHT20 sensor object.
     * @param i2cDriver A pointer to the I2C driver for communication.
     */
    explicit AHT20(I2CDriver *i2cDriver, QObject *parent = nullptr);
    ~AHT20();

    /**
     * @brief Initializes the AHT20 sensor.
     * @return true if initialization is successful, false otherwise.
     */
    bool initialize();

public slots:
    /**
     * @brief Monitors the sensor for new data and triggers measurements.
     *
     * This function runs continuously in a separate thread.
     */
    void startMonitorSensor();
    void stopMonitoring();

signals:
    /**
     * @brief Emitted when new temperature and humidity data is available.
     * @param temperature The measured temperature in ��C.
     * @param humidity The measured humidity in %.
     */
    void dataReady(float temperature, float humidity);

private:
    I2CDriver *i2cDriver_;  ///< I2C communication driver.
    QAtomicInt keepRunning_;
    int deviceAddress_;
    volatile bool isReading;

    /**
     * @brief Checks if new sensor data is available.
     * @return true if data is ready, false otherwise.
     */
    bool checkSensorReady();
};

#endif // AHT20_H
