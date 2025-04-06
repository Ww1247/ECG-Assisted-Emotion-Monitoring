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

/**
 * @brief The TemperatureHumidityWidget class displays real-time temperature and humidity trends.
 *
 * It uses QCustomPlot to draw two independent plots and supports time-based updates via PlotRefreshInterface.
 */
class TemperatureHumidityWidget : public QWidget, public PlotRefreshInterface
{
    Q_OBJECT

public:
    /**
     * @brief Constructs the TemperatureHumidityWidget.
     * @param parent The parent QWidget.
     */
    explicit TemperatureHumidityWidget(QWidget *parent = nullptr);

    /**
     * @brief Updates the plot data and refreshes the display.
     */
    void updatePlot() override;

public slots:
    /**
     * @brief Adds new temperature and humidity data points to the plot buffer.
     * @param temperature Temperature value in degrees Celsius.
     * @param humidity Relative humidity percentage.
     */
    void addData(float temperature, float humidity);  // External call

private:
    QCustomPlot *customPlot_temperature; ///< Plot widget for temperature data.
    QCustomPlot *customPlot_humidity;    ///< Plot widget for humidity data.

    QVector<double> timeData_;           ///< X-axis data (time in seconds since start).
    QVector<double> temperatureData_;    ///< Y-axis temperature data.
    QVector<double> humidityData_;       ///< Y-axis humidity data.

    QTimer *updateTimer_;                ///< Timer used for triggering periodic plot updates.
    QElapsedTimer elapsed_;              ///< Timer tracking elapsed time since widget initialization.

    const int maxPoints_ = 300;          ///< Maximum number of data points to retain for plotting.

    /**
     * @brief Initializes the UI layout and widgets.
     */
    void initUI();

    /**
     * @brief Configures a QCustomPlot instance with axis labels and ranges.
     * @param plot Pointer to the QCustomPlot instance.
     */
    void setupPlot(QCustomPlot *plot);

    /**
     * @brief Redraws the plots based on the latest buffered data.
     */
    void updatePlots();
};


#endif // TEMPERATURE_HUMIDITY_H
