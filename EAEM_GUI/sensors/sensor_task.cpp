#include "sensor_task.h"
#include <QThread>
#include <QDebug>

SensorTask::SensorTask(const QString &name, SensorInterface *sensor, std::atomic_bool *stopFlag)
    : name_(name), sensor_(sensor), stopFlag_(stopFlag)
{
    setAutoDelete(true);
}

void SensorTask::run()
{
    qDebug() << "[SensorTask]" << name_ << "running in thread:" << QThread::currentThread();

    if (!sensor_) {
        emit errorOccurred(name_, "Sensor is null.");
        return;
    }

    while (!stopFlag_->load()) {
        SensorData data;
        data.sensor_name = name_;
        data.timestamp = QDateTime::currentDateTime();

        if (sensor_->readOnce(data)) {
            emit dataReady(data);
        } else {
            emit errorOccurred(name_, "Read failed.");
        }
        QThread::msleep(10);
    }

    qDebug() << "[SensorTask]" << name_ << "stopped.";
}
