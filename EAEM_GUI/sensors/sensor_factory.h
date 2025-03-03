#ifndef I2C_DRIVER_FACTORY_H
#define I2C_DRIVER_FACTORY_H

#include "i2c_driver.h"
#include <QMap>
#include <QThread>

class I2CDriverFactory {
public:
    static I2CDriver* createDriver(int deviceAddress);
    static void releaseAllDrivers(); // Optional: method to clean up all instances on program exit

private:
    static QMap<int, I2CDriver*> drivers;
    static QMap<int, QThread*> threads;
};

#endif // I2C_DRIVER_FACTORY_H
