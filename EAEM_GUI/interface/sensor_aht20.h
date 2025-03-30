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

class SensorAHT20Widget : public QWidget {

    Q_OBJECT

public:
    explicit SensorAHT20Widget(QWidget *parent = nullptr);
    ~SensorAHT20Widget();

public slots:
    void updateValues(const float &temperature, const float &humidity);

private:
    void initUI();

    QLineEdit *lineEdit_temperature_value;
    QLineEdit *lineEdit_humidity_value;

};

#endif // SENSOR_AHT20_H
