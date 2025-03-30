#include "sensor_manager.h"
#include <QDebug>

SensorManager::SensorManager(QObject *parent) : QObject(parent) {}

SensorManager::~SensorManager() {
    stopAll();
}

void SensorManager::registerSensor(const QString &name, SensorInterface *sensor)
{
    if (!sensors_.contains(name)) {
        auto entry = QSharedPointer<SensorEntry>::create(sensor);
        sensors_.insert(name, entry);
        qDebug() << "[SensorManager] Registered sensor:" << name;
    }
}

void SensorManager::startSensor(const QString &name)
{
    auto it = sensors_.find(name);
    if (it == sensors_.end()) {
        qWarning() << "[SensorManager] Sensor" << name << "not found in map.";
        return;
    }
    QSharedPointer<SensorEntry> entry = it.value();
    if (!entry->sensor->initialize()) {
        emit errorOccurred(name, "Initialization failed.");
        return;
    }
    entry->stopFlag.store(false);
    SensorTask *task = new SensorTask(name, entry->sensor, &entry->stopFlag);
    connect(task, &SensorTask::dataReady, this, &SensorManager::dataReady);
    connect(task, &SensorTask::errorOccurred, this, &SensorManager::errorOccurred);
    QThreadPool::globalInstance()->start(task);
    qDebug() << "[SensorManager] Started sensor:" << name;
}

void SensorManager::stopSensor(const QString &name)
{
    auto it = sensors_.find(name);
    if (it != sensors_.end()) {
        QSharedPointer<SensorEntry> entry = it.value();
        entry->stopFlag.store(true);
        qDebug() << "[SensorManager] Stopped sensor:" << name;
    }
}

void SensorManager::stopAll()
{
    for (auto it = sensors_.begin(); it != sensors_.end(); ++it) {
        QSharedPointer<SensorEntry> entry = it.value();
        entry->stopFlag.store(true);
        qDebug() << "[SensorManager] Stopped sensor:" << it.key();
    }
}

bool SensorManager::isRunning(const QString &name) const
{
    auto it = sensors_.find(name);
    return (it != sensors_.end()) && !it.value()->stopFlag.load();
}

void SensorManager::sendControlCommand(const QString &name, const QVariantMap &params)
{
    if (sensors_.contains(name)) {
        sensors_[name]->sensor->applySetting(params);
        qDebug() << "[SensorManager] Control command sent to" << name << ":" << params;
    }
}
