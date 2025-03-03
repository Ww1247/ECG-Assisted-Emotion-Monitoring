#include "sensor_factory.h"

QMap<int, I2CDriver*> I2CDriverFactory::drivers;
QMap<int, QThread*> I2CDriverFactory::threads;

I2CDriver* I2CDriverFactory::createDriver(int deviceAddress) {
    if (!drivers.contains(deviceAddress)) {
        QThread* thread = new QThread();
        I2CDriver* driver = new I2CDriver(deviceAddress);
        driver->moveToThread(thread);
        threads[deviceAddress] = thread;
        drivers[deviceAddress] = driver;
        thread->start();
    }
    return drivers[deviceAddress];
}

void I2CDriverFactory::releaseAllDrivers() {
    auto iter = threads.begin();
    while (iter != threads.end()) {
        if (iter.value()) {
            iter.value()->quit();
            iter.value()->wait();
            delete iter.value();
        }
        ++iter;
    }
    threads.clear();

    iter = drivers.begin();
    while (iter != drivers.end()) {
        if (iter.value()) {
            delete iter.value();
        }
        ++iter;
    }
    drivers.clear();
}
