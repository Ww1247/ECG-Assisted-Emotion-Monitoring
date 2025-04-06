#ifndef I2C_DRIVER_H
#define I2C_DRIVER_H

#include <QObject>
#include <QMutex>
#include <QMutexLocker>
#include <QByteArray>
#include <pigpio.h>

/**
 * @brief I2C driver class based on Qt, using the pigpio library for I2C communication.
 *
 * This class encapsulates I2C read/write operations and provides thread-safe APIs
 * to support I2C operations in a multi-threaded environment.
 */
class I2CDriver : public QObject
{
    Q_OBJECT

public:

    /**
     * @brief Constructs an I2CDriver object.
     * @param deviceAddress The I2C device address.
     */
    explicit I2CDriver(int deviceAddress, QObject *parent = nullptr);

    /**
     * @brief Destructs the I2CDriver object.
     * Closes the I2C handle.
     */
    ~I2CDriver();

    /**
     * @brief Initializes the I2C device.
     * @return true if initialization is successful, false otherwise.
     */
    bool initialize();

    /**
     * @brief Closes the I2C device.
     * @return true if successful, false otherwise.
     */
    bool close();

    /**
     * @brief Writes a byte to the specified register.
     * @param reg The register address.
     * @param value The byte value to write.
     * @return true if successful, false otherwise.
     */
    bool writeByte(quint8 reg, quint8 value);

    /**
     * @brief Writes multiple bytes to the specified register.
     * @param reg The register address.
     * @param values The data to write.
     * @return true if successful, false otherwise.
     */
    bool writeBytes(quint8 reg, const QByteArray &values);

    /**
     * @brief Reads a byte from the specified register.
     * @param reg The register address.
     * @param value The read byte will be stored here.
     * @return true if successful, false otherwise.
     */
    bool readByte(quint8 reg, quint8 &value);

    /**
     * @brief Reads multiple bytes from the specified register.
     * @param reg The register address.
     * @param count The number of bytes to read.
     * @return A QVector containing the read data.
     */
    QByteArray readBytes(quint8 reg, quint8 count);

private:

    static QMutex i2cMutex_;     ///< Qt thread lock to ensure thread safety in I2C operations
    int i2cHandle_;              ///< I2C handle returned by pigpio
    int deviceAddress_;          ///< I2C device address

    /**
     * @brief Converts an integer to a hexadecimal string.
     * @param value The integer value to convert.
     * @return The corresponding hexadecimal string.
     */
    QString intToHex(int value);

signals:

    void errorOccurred(const QString &error_message);
};

#endif // I2C_DRIVER_H
