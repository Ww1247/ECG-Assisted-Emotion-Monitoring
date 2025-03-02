#include "temperature_humidity.h"

TemperatureHumidityWidget::TemperatureHumidityWidget(QWidget *parent) : QWidget(parent) {
    initUI();
}

void TemperatureHumidityWidget::initUI() {
    // Temperature Group Box
    QGroupBox *groupBox_temperature = new QGroupBox("Temperature", this);

    QFrame *frame_temperature = new QFrame;
    frame_temperature->setMinimumSize(450, 150);
    frame_temperature->setFrameShape(QFrame::Box);

    QVBoxLayout *layout_temperature = new QVBoxLayout;
    layout_temperature->addWidget(frame_temperature);
    groupBox_temperature->setLayout(layout_temperature);

    // Humidity Group Box
    QGroupBox *groupBox_humidity = new QGroupBox("Humidity", this);

    QFrame *frame_humidity = new QFrame;
    frame_humidity->setMinimumSize(450, 150);
    frame_humidity->setFrameShape(QFrame::Box);

    QVBoxLayout *layout_humidity = new QVBoxLayout;
    layout_humidity->addWidget(frame_humidity);
    groupBox_humidity->setLayout(layout_humidity);

    // Main Layout
    QHBoxLayout *horizontalLayout_AHT20 = new QHBoxLayout;
    horizontalLayout_AHT20->addWidget(groupBox_temperature);
    horizontalLayout_AHT20->addWidget(groupBox_humidity);

    setLayout(horizontalLayout_AHT20);
}
