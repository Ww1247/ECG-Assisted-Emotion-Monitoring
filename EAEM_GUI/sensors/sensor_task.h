#ifndef SENSORTASK_H
#define SENSORTASK_H

#pragma once
#include <QObject>
#include <QRunnable>
#include <QPointer>
#include <atomic>
#include "sensor_interface.h"
#include "sensor_data.h"

class SensorTask : public QObject, public QRunnable {
    Q_OBJECT

public:
    SensorTask(const QString &name, SensorInterface *sensor, std::atomic_bool *stopFlag);

    void run() override;

signals:
    void dataReady(const SensorData &data);
    void errorOccurred(QString name, const QString &errorMsg);

private:
    QString name_;
    SensorInterface *sensor_;
    std::atomic_bool *stopFlag_;
};

#endif // SENSORTASK_H
