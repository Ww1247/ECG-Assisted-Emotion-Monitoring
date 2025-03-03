#include "Max30102.h"
#include <QByteArray>

Max30102::Max30102(I2CDriver *driver, QObject *parent) 
    : QObject(parent), i2cDriver(driver) {
    dataTimer = new QTimer(this);
    connect(dataTimer, &QTimer::timeout, this, &Max30102::updateSensorData);
}

Max30102::~Max30102() {
    dataTimer->stop();
    delete dataTimer;
}

void Max30102::start() {
    configureSensor();  // 配置传感器
    dataTimer->start(1000);  // 每秒更新一次数据
}

void Max30102::updateSensorData() {
    QByteArray data;
    try {
        data = i2cDriver->readBytes(0x00, 6);  // 假设从0x00寄存器读取6字节数据
        emit newDataAvailable(data);
    } catch (std::runtime_error &e) {
        emit errorOccurred(QString::fromStdString(e.what()));
    }
}

void Max30102::configureSensor() {
    try {
        i2cDriver->writeByte(0x01, 0x03);  // 配置寄存器
    } catch (std::runtime_error &e) {
        emit errorOccurred(QString::fromStdString(e.what()));
    }
}
