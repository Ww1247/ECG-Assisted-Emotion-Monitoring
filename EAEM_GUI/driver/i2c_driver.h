#ifndef I2C_DRIVER_H
#define I2C_DRIVER_H

#include <QObject>

class I2C_Driver : public QObject
{
    Q_OBJECT
public:
    explicit I2C_Driver(QObject *parent = nullptr);

signals:
};

#endif // I2C_DRIVER_H
