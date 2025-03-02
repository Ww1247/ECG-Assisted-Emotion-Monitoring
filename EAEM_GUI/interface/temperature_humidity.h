#ifndef TEMPERATURE_HUMIDITY_H
#define TEMPERATURE_HUMIDITY_H

#include <QWidget>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFrame>
#include "qcustomplot.h"

class TemperatureHumidityWidget : public QWidget {
    Q_OBJECT
public:
    explicit TemperatureHumidityWidget(QWidget *parent = nullptr);

private:
    void initUI();
    void setupPlot(QCustomPlot *plot);
};

#endif // TEMPERATURE_HUMIDITY_H
