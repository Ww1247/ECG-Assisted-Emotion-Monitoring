#ifndef SENSOR_AHT20_H
#define SENSOR_AHT20_H

#include <QWidget>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QSpacerItem>
#include <QThread>

#include "i2c_driver.h"
#include "aht20.h"

class SensorAHT20Widget : public QWidget {

    Q_OBJECT

public:
    explicit SensorAHT20Widget(QWidget *parent = nullptr);
    ~SensorAHT20Widget();

    void sensorReadStart();
    void sensorReadStop();

private:
    void initUI();
    void startSensorThread();

    I2CDriver *i2cDriver_;
    AHT20 *aht20_;
    QThread *sensorThread_;

    QLineEdit *lineEdit_temperature_value;
    QLineEdit *lineEdit_humidity_value;

private slots:
    void onDataReady(float temperature, float humidity);

signals:
    void sig_errorOccurred(const QStringList &error_message);

};

#endif // SENSOR_AHT20_H
