#include "temperature_humidity.h"

TemperatureHumidityWidget::TemperatureHumidityWidget(QWidget *parent) : QWidget(parent) {
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
    QCustomPlot *customPlot_temperature = new QCustomPlot(frame_temperature);
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
    QCustomPlot *customPlot_humidity = new QCustomPlot(frame_humidity);
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
}

// Function to initialize QCustomPlot settings
void TemperatureHumidityWidget::setupPlot(QCustomPlot *plot) {
    plot->addGraph();
    plot->graph(0)->setPen(QPen(Qt::red));  // Set plot line color

    plot->xAxis->setRange(0, 10);
    plot->yAxis->setRange(0, 100);
}
