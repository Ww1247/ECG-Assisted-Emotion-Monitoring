#include "sensor_max30102.h"
#include <QDebug>

#define MAX30102_I2C_ADDRESS 0x57

SensorMAX30102Widget::SensorMAX30102Widget(QWidget *parent)
    : QWidget(parent)
{
    initUI();

//    i2cDriver_ = new MAX30102(0x57);
//    sensorThread = new QThread(this);
//    i2cDriver_->moveToThread(sensorThread);

//    connect(, QOverload<int>::of(&QComboBox::currentIndexChanged),
//               [=](int index){ sensor->setMode(static_cast<quint8>(index + 2)); });

//    connect(ui->samplingRateSlider, &QSlider::valueChanged,
//               [=](int value){ sensor->setSamplingRate(static_cast<quint8>(value)); });

//    connect(ui->ledBrightnessSlider, &QSlider::valueChanged,
//               [=](int value){ sensor->setLEDPulseAmplitude(static_cast<quint8>(value)); });

}

SensorMAX30102Widget::~SensorMAX30102Widget()
{
//    sensorReadStop();
}


void SensorMAX30102Widget::initUI()
{
    // Create a group box for the MAX30102 sensor
    QGroupBox *groupBox_sensor_max30102 = new QGroupBox("MAX30102", this);
    groupBox_sensor_max30102->setGeometry(120, 120, 624, 152);

    // Create checkboxes for mode selection (mutually exclusive)
    checkBox_heart_rate_mode = new QCheckBox("Heart Rate Mode", this);
    checkBox_blood_oxygen_mode = new QCheckBox("SpO2 Mode", this);
    checkBox_mixed_mode = new QCheckBox("Mixed Mode", this);
    checkBox_mixed_mode->setChecked(true);  // Set mixed mode as default

    checkBox_heart_rate_mode->setAutoExclusive(true);
    checkBox_blood_oxygen_mode->setAutoExclusive(true);
    checkBox_mixed_mode->setAutoExclusive(true);

    // Connect checkbox toggles to the handler function
    connect(checkBox_heart_rate_mode, &QCheckBox::clicked, this, &SensorMAX30102Widget::on_checkBox_mode_changed);
    connect(checkBox_blood_oxygen_mode, &QCheckBox::clicked, this, &SensorMAX30102Widget::on_checkBox_mode_changed);
    connect(checkBox_mixed_mode, &QCheckBox::clicked, this, &SensorMAX30102Widget::on_checkBox_mode_changed);

    // Layout for mode selection checkboxes
    QHBoxLayout *horizontalLayout_mode_config = new QHBoxLayout;
    horizontalLayout_mode_config->addWidget(checkBox_heart_rate_mode);
    horizontalLayout_mode_config->addWidget(checkBox_blood_oxygen_mode);
    horizontalLayout_mode_config->addWidget(checkBox_mixed_mode);
    horizontalLayout_mode_config->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

    // Heart rate display components
    QLabel *label_heart_rate = new QLabel("Heart Rate:");
    label_heart_rate->setMinimumSize(90, 0);
    QLineEdit *lineEdit_heart_rate_value = new QLineEdit;
    lineEdit_heart_rate_value->setMinimumSize(100, 0);
    lineEdit_heart_rate_value->setAlignment(Qt::AlignCenter);
    lineEdit_heart_rate_value->setReadOnly(true);
    QLabel *icon_heart_rate = new QLabel;
    icon_heart_rate->setMinimumSize(22, 22);
    icon_heart_rate->setMaximumSize(22, 22);

    // Layout for heart rate display
    QHBoxLayout *horizontalLayout_heart_rate = new QHBoxLayout;
    horizontalLayout_heart_rate->addWidget(label_heart_rate);
    horizontalLayout_heart_rate->addWidget(lineEdit_heart_rate_value);
    horizontalLayout_heart_rate->addWidget(icon_heart_rate);

    // Slider for adjusting heart rate values
    slider_heart_rate = new QSlider(Qt::Horizontal, this);
    slider_heart_rate->setRange(40, 180);  // Heart rate range: 40-180 BPM
    connect(slider_heart_rate, &QSlider::valueChanged, this, &SensorMAX30102Widget::on_slider_heart_rate_changed);

    QVBoxLayout *verticalLayout_heart_rate = new QVBoxLayout;
    verticalLayout_heart_rate->addLayout(horizontalLayout_heart_rate);
    verticalLayout_heart_rate->addWidget(slider_heart_rate);

    // Blood oxygen display components
    QLabel *label_blood_oxygen = new QLabel("Blood Oxygen:");
    label_blood_oxygen->setMinimumSize(90, 0);
    QLineEdit *lineEdit_blood_oxygen_value = new QLineEdit;
    lineEdit_blood_oxygen_value->setMinimumSize(100, 0);
    lineEdit_blood_oxygen_value->setAlignment(Qt::AlignCenter);
    lineEdit_blood_oxygen_value->setReadOnly(true);
    QLabel *icon_blood_oxygen = new QLabel;
    icon_blood_oxygen->setMinimumSize(22, 22);
    icon_blood_oxygen->setMaximumSize(22, 22);

    // Layout for blood oxygen display
    QHBoxLayout *horizontalLayout_blood_oxygen = new QHBoxLayout;
    horizontalLayout_blood_oxygen->addWidget(label_blood_oxygen);
    horizontalLayout_blood_oxygen->addWidget(lineEdit_blood_oxygen_value);
    horizontalLayout_blood_oxygen->addWidget(icon_blood_oxygen);

    // Slider for adjusting blood oxygen levels
    slider_blood_oxygen = new QSlider(Qt::Horizontal, this);
    slider_blood_oxygen->setRange(70, 100);  // Blood oxygen range: 70-100%
    connect(slider_blood_oxygen, &QSlider::valueChanged, this, &SensorMAX30102Widget::on_slider_blood_oxygen_changed);

    QVBoxLayout *verticalLayout_blood_oxygen = new QVBoxLayout;
    verticalLayout_blood_oxygen->addLayout(horizontalLayout_blood_oxygen);
    verticalLayout_blood_oxygen->addWidget(slider_blood_oxygen);

    // Layout for sensor value displays
    QHBoxLayout *horizontalLayout_sensor_value_display = new QHBoxLayout;
    horizontalLayout_sensor_value_display->addLayout(verticalLayout_heart_rate);
    horizontalLayout_sensor_value_display->addLayout(verticalLayout_blood_oxygen);

    // Main layout
    QVBoxLayout *verticalLayout_max30102 = new QVBoxLayout;
    verticalLayout_max30102->addLayout(horizontalLayout_mode_config);
    verticalLayout_max30102->addLayout(horizontalLayout_sensor_value_display);

    groupBox_sensor_max30102->setLayout(verticalLayout_max30102);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(groupBox_sensor_max30102);
    setLayout(mainLayout);
}

void SensorMAX30102Widget::on_comboBox_sampling_rate_currentIndexChanged(int index)
{
    qDebug() << "Sampling Rate Index" << index;
}

void SensorMAX30102Widget::on_comboBox_led_red_currentIndexChanged(int index)
{
    qDebug() << "LED RED Index" << index;
}

void SensorMAX30102Widget::on_comboBox_led_ir_currentIndexChanged(int index)
{
    qDebug() << "LED IR Index" << index;
}

// Slot function for heart rate slider value change
void SensorMAX30102Widget::on_slider_heart_rate_changed(int value)
{
    qDebug() << "Heart Rate:" << value;
}

// Slot function for blood oxygen slider value change
void SensorMAX30102Widget::on_slider_blood_oxygen_changed(int value)
{
    qDebug() << "Blood Oxygen:" << value << "%";
}

// Slot function to handle mode selection
void SensorMAX30102Widget::on_checkBox_mode_changed(bool checked)
{
//    if (!checked) return;  // Ignore unchecked state

//    if (checkBox_heart_rate_mode->isChecked()) {
//        qDebug() << "Heart Rate Mode Selected";
//        i2cDriver_->setMode(0x02);
//    } else if (checkBox_blood_oxygen_mode->isChecked()) {
//        qDebug() << "SpO2 Mode Selected";
//        i2cDriver_->setMode(0x03);
//    } else if (checkBox_mixed_mode->isChecked()) {
//        qDebug() << "Mixed Mode Selected";
//        i2cDriver_->setMode(0x07);
//    }
}

bool SensorMAX30102Widget::sensorReadStart()
{
    qDebug() << "MAX30102 Sensor Read Start.";
    return true;
}

bool SensorMAX30102Widget::sensorReadStop()
{
    qDebug() << "MAX30102 Sensor Read Stop.";
    return true;
}
