#include "temperature_humidity.h"

TemperatureHumidityWidget::TemperatureHumidityWidget(QWidget *parent)
    : QWidget(parent)
{
    initUI();
}

void TemperatureHumidityWidget::initUI() {
    // Temperature Group Box
    QGroupBox *groupBox_temperature = new QGroupBox("Temperature", this);

    // Temperature Display Frame
    QFrame *frame_temperature = new QFrame;
    frame_temperature->setMinimumSize(450, 150);
    frame_temperature->setFrameShape(QFrame::Box);

    QVBoxLayout *layout_frame_temperature = new QVBoxLayout(frame_temperature);
    layout_frame_temperature->setContentsMargins(0, 0, 0, 0);
    layout_frame_temperature->setSpacing(0);

    // QCustomPlot inside Frame
    customPlot_temperature = new QCustomPlot(frame_temperature);
    customPlot_temperature->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    layout_frame_temperature->addWidget(customPlot_temperature);

    // Initialize Temperature Plot
    setupPlot(customPlot_temperature);

    // Layout for Temperature Group Box
    QVBoxLayout *layout_temperature = new QVBoxLayout;
    layout_temperature->addWidget(frame_temperature);
    groupBox_temperature->setLayout(layout_temperature);

    // Humidity Group Box
    QGroupBox *groupBox_humidity = new QGroupBox("Humidity", this);

    // Humidity Display Frame
    QFrame *frame_humidity = new QFrame;
    frame_humidity->setMinimumSize(450, 150);
    frame_humidity->setFrameShape(QFrame::Box);

    QVBoxLayout *layout_frame_humidity = new QVBoxLayout(frame_humidity);
    layout_frame_humidity->setContentsMargins(0, 0, 0, 0);
    layout_frame_humidity->setSpacing(0);

    // QCustomPlot inside Frame
    customPlot_humidity = new QCustomPlot(frame_humidity);
    customPlot_humidity->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    layout_frame_humidity->addWidget(customPlot_humidity);

    // Initialize Humidity Plot
    setupPlot(customPlot_humidity);

    // Layout for Humidity Group Box
    QVBoxLayout *layout_humidity = new QVBoxLayout;
    layout_humidity->addWidget(frame_humidity);
    groupBox_humidity->setLayout(layout_humidity);

    // Main Layout
    QHBoxLayout *horizontalLayout_TemperatureHumidity = new QHBoxLayout;
    horizontalLayout_TemperatureHumidity->addWidget(groupBox_temperature);
    horizontalLayout_TemperatureHumidity->addWidget(groupBox_humidity);

    setLayout(horizontalLayout_TemperatureHumidity);

    // Initialize the timestamp
    elapsed_.start();
}

// Function to initialize QCustomPlot settings
void TemperatureHumidityWidget::setupPlot(QCustomPlot *plot) {
    plot->addGraph();
    plot->graph(0)->setPen(QPen(Qt::red));  // Set plot line color

     // Configure X axis (time in milliseconds)
    plot->xAxis->setTickLabels(false);
    plot->xAxis->setLabel("Time (ms)"); // Set X-axis label

    // Configure Y axis
    plot->yAxis->setTickLabels(true);
    plot->yAxis->setRange(0, 100);

    // Enable user interactions for zooming and panning
    plot->setInteraction(QCP::iRangeDrag, true);
    plot->setInteraction(QCP::iRangeZoom, true);

    // Prevent the view from resetting during data updates
    // customPlot->axisRect()->setRangeZoom(Qt::Horizontal | Qt::Vertical);
    // customPlot->axisRect()->setRangeDrag(Qt::Horizontal | Qt::Vertical);
    plot->axisRect()->setRangeZoom(Qt::Horizontal);
    plot->axisRect()->setRangeDrag(Qt::Horizontal);
}

void TemperatureHumidityWidget::addData(float temperature, float humidity)
{
    double currentTime = elapsed_.elapsed();  // ms

    timeData_.append(currentTime);
    temperatureData_.append(temperature);
    humidityData_.append(humidity);

    // Control the maximum number of data points
    if (timeData_.size() > maxPoints_) {
        timeData_.remove(0, timeData_.size() - maxPoints_);
        temperatureData_.remove(0, temperatureData_.size() - maxPoints_);
        humidityData_.remove(0, humidityData_.size() - maxPoints_);
    }
}

void TemperatureHumidityWidget::updatePlots()
{
    if (timeData_.isEmpty()) return;

    // Set data
    customPlot_temperature->graph(0)->setData(timeData_, temperatureData_);
    customPlot_humidity->graph(0)->setData(timeData_, humidityData_);

    // Automatically moves the X-axis to display the last data segment
    double latestTime = timeData_.last();
    customPlot_temperature->xAxis->setRange(latestTime - 5000, latestTime);  // Show the last 5s
    customPlot_humidity->xAxis->setRange(latestTime - 5000, latestTime);

    // Automatic Y-axis scaling (optional or fixed range)
    customPlot_temperature->yAxis->rescale();
    customPlot_humidity->yAxis->rescale();

    // Refresh
    customPlot_temperature->replot();
    customPlot_humidity->replot();
}

void TemperatureHumidityWidget::updatePlot()
{
    updatePlots();
}
