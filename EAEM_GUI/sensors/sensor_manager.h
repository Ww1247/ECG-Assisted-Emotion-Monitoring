#ifndef SENSORMANAGER_H
#define SENSORMANAGER_H

#pragma once
#include <QObject>
#include <QMap>
#include <QThreadPool>
#include <QSharedPointer>
#include <atomic>
#include "sensor_data.h"
#include "sensor_task.h"
#include "sensor_interface.h"

class SensorManager : public QObject {
    Q_OBJECT

public:
    explicit SensorManager(QObject *parent = nullptr);
    ~SensorManager();

    void registerSensor(const QString &name, SensorInterface *sensor);
    void startSensor(const QString &name);
    void stopSensor(const QString &name);
    void stopAll();
    bool isRunning(const QString &name) const;
    void sendControlCommand(const QString &name, const QVariantMap &params);

signals:
    void dataReady(const SensorData &data);
    void errorOccurred(QString name, const QString &msg);

private:
    struct SensorEntry {
        SensorInterface *sensor;
        std::atomic_bool stopFlag;
        SensorEntry(SensorInterface *s)
            : sensor(s), stopFlag(false) {}
    };

    QMap<QString, QSharedPointer<SensorEntry>> sensors_;
};

#endif // SENSORMANAGER_H
