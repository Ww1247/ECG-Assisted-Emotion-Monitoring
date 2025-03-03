#ifndef MAX30102_H
#define MAX30102_H

#include <QObject>
#include <QTimer>
#include "i2c_driver.h"

class Max30102 : public QObject {
    Q_OBJECT

public:
    explicit Max30102(I2CDriver *driver, QObject *parent = nullptr);
    ~Max30102();

    void start();  // 开始连续读取数据

signals:
    void newDataAvailable(const QByteArray &data);  // 新数据可用
    void errorOccurred(const QString &error);  // 错误信息

private slots:
    void updateSensorData();  // 更新传感器数据

private:
    I2CDriver *i2cDriver;  // I2C 通信类
    QTimer *dataTimer;     // 定时器，用于定时读取数据

    void configureSensor();  // 配置传感器的函数
};

#endif // MAX30102_H
