#include "sensor_aht20.h"
#include <QDebug>

#define AHT20_I2C_ADDRESS  0x38

SensorAHT20Widget::SensorAHT20Widget(QWidget *parent)
    : QWidget(parent)
{
    initUI();
}

SensorAHT20Widget::~SensorAHT20Widget() {
    qDebug() << "[LOG]: SensorAHT20Widget Distruct";
}

void SensorAHT20Widget::initUI()
{
    // Create AHT20 sensor group box
    QGroupBox *groupBox_sensor_aht20 = new QGroupBox("AHT20", this);
    groupBox_sensor_aht20->setGeometry(120, 190, 624, 100);

    // Temperature display
    QLabel *label_temperature = new QLabel("Temperature:");
    label_temperature->setMinimumSize(90, 0);
    lineEdit_temperature_value = new QLineEdit;
    lineEdit_temperature_value->setMinimumSize(100, 0);
    lineEdit_temperature_value->setAlignment(Qt::AlignCenter);
    lineEdit_temperature_value->setReadOnly(true);
    QLabel *icon_temperature = new QLabel;
    icon_temperature->setMinimumSize(22, 22);
    icon_temperature->setMaximumSize(22, 22);

    // Layout for temperature display
    QHBoxLayout *horizontalLayout_temperature = new QHBoxLayout;
    horizontalLayout_temperature->addWidget(label_temperature);
    horizontalLayout_temperature->addWidget(lineEdit_temperature_value);
    horizontalLayout_temperature->addWidget(icon_temperature);

    // Temperature threshold settings
    QLabel *label_temperature_max = new QLabel("Max:");
    label_temperature_max->setMinimumSize(30, 0);
    QDoubleSpinBox *doubleSpinBox_temperature_threshold_max = new QDoubleSpinBox;

    QLabel *label_temperature_min = new QLabel("Min:");
    label_temperature_min->setMinimumSize(30, 0);
    QDoubleSpinBox *doubleSpinBox_temperature_threshold_min = new QDoubleSpinBox;

    // Layout for temperature threshold settings
    QHBoxLayout *horizontalLayout_temperature_threshold = new QHBoxLayout;
    horizontalLayout_temperature_threshold->addWidget(label_temperature_max);
    horizontalLayout_temperature_threshold->addWidget(doubleSpinBox_temperature_threshold_max);
    horizontalLayout_temperature_threshold->addWidget(label_temperature_min);
    horizontalLayout_temperature_threshold->addWidget(doubleSpinBox_temperature_threshold_min);
    horizontalLayout_temperature_threshold->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

    // Vertical layout for temperature section
    QVBoxLayout *verticalLayout_temperature = new QVBoxLayout;
    verticalLayout_temperature->addLayout(horizontalLayout_temperature);
    verticalLayout_temperature->addLayout(horizontalLayout_temperature_threshold);

    // Humidity display
    QLabel *label_humidity = new QLabel("Humidity:");
    label_humidity->setMinimumSize(90, 0);
    lineEdit_humidity_value = new QLineEdit;
    lineEdit_humidity_value->setMinimumSize(100, 0);
    lineEdit_humidity_value->setAlignment(Qt::AlignCenter);
    lineEdit_humidity_value->setReadOnly(true);
    QLabel *icon_humidity = new QLabel;
    icon_humidity->setMinimumSize(22, 22);
    icon_humidity->setMaximumSize(22, 22);

    // Layout for humidity display
    QHBoxLayout *horizontalLayout_humidity = new QHBoxLayout;
    horizontalLayout_humidity->addWidget(label_humidity);
    horizontalLayout_humidity->addWidget(lineEdit_humidity_value);
    horizontalLayout_humidity->addWidget(icon_humidity);

    // Humidity threshold settings
    QLabel *label_humidity_threshold_max = new QLabel("Max:");
    label_humidity_threshold_max->setMinimumSize(30, 0);
    QDoubleSpinBox *doubleSpinBox_humidity_threshold_max = new QDoubleSpinBox;

    QLabel *label_humidity_threshold_min = new QLabel("Min:");
    label_humidity_threshold_min->setMinimumSize(30, 0);
    QDoubleSpinBox *doubleSpinBox_humidity_threshold_min = new QDoubleSpinBox;

    // Layout for humidity threshold settings
    QHBoxLayout *horizontalLayout_humidity_threshold = new QHBoxLayout;
    horizontalLayout_humidity_threshold->addWidget(label_humidity_threshold_max);
    horizontalLayout_humidity_threshold->addWidget(doubleSpinBox_humidity_threshold_max);
    horizontalLayout_humidity_threshold->addWidget(label_humidity_threshold_min);
    horizontalLayout_humidity_threshold->addWidget(doubleSpinBox_humidity_threshold_min);
    horizontalLayout_humidity_threshold->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

    // Vertical layout for humidity section
    QVBoxLayout *verticalLayout_humidity = new QVBoxLayout;
    verticalLayout_humidity->addLayout(horizontalLayout_humidity);
    verticalLayout_humidity->addLayout(horizontalLayout_humidity_threshold);

    // Sensor divider line
    QFrame *line_aht20 = new QFrame;
    line_aht20->setFrameShape(QFrame::VLine);
    line_aht20->setFrameShadow(QFrame::Sunken);

    // Horizontal layout for AHT20 sensor
    QHBoxLayout *horizontalLayout_aht20 = new QHBoxLayout;
    horizontalLayout_aht20->addLayout(verticalLayout_temperature);
    horizontalLayout_aht20->addWidget(line_aht20);
    horizontalLayout_aht20->addLayout(verticalLayout_humidity);

    // Main layout for AHT20 sensor group
    QGridLayout *gridLayout_3 = new QGridLayout;
    gridLayout_3->addLayout(horizontalLayout_aht20, 0, 0);
    groupBox_sensor_aht20->setLayout(gridLayout_3);

    // Set the final layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(groupBox_sensor_aht20);
    setLayout(mainLayout);
}

void SensorAHT20Widget::updateValues(const float &temperature, const float &humidity)
{
    this->lineEdit_temperature_value->setText(QString::number(temperature, 'f', 3));
    this->lineEdit_humidity_value->setText(QString::number(humidity, 'f', 3));
}
