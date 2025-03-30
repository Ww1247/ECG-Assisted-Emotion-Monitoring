#ifndef TEMPERATURE_HUMIDITY_H
#define TEMPERATURE_HUMIDITY_H

#include <QWidget>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFrame>
#include "qcustomplot.h"
#include "plot_refresh_manager.h"

class TemperatureHumidityWidget : public QWidget, public PlotRefreshInterface
{
    Q_OBJECT

public:
    explicit TemperatureHumidityWidget(QWidget *parent = nullptr);
    void updatePlot() override;

public slots:
    void addData(float temperature, float humidity);  // External call

private:
    QCustomPlot *customPlot_temperature;
    QCustomPlot *customPlot_humidity;
    QVector<double> timeData_;         // X-axis: Timestamp
    QVector<double> temperatureData_;  // Temperature curve
    QVector<double> humidityData_;     // Humidity curve
    QTimer *updateTimer_;              // Timed refresh drawing
    QElapsedTimer elapsed_;            // Start time
    const int maxPoints_ = 300;        // Maximum number of data points retained

    void initUI();
    void setupPlot(QCustomPlot *plot);
    void updatePlots();
};

#endif // TEMPERATURE_HUMIDITY_H
