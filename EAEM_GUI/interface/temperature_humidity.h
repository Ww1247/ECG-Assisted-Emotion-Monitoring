#ifndef TEMPERATURE_HUMIDITY_H
#define TEMPERATURE_HUMIDITY_H

#include <QWidget>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFrame>

class TemperatureHumidityWidget : public QWidget {
    Q_OBJECT
public:
    explicit TemperatureHumidityWidget(QWidget *parent = nullptr);

private:
    void initUI();
};

#endif // TEMPERATURE_HUMIDITY_H
