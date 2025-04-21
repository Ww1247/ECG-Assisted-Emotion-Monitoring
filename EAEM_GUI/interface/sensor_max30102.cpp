#include "sensor_max30102.h"
#include <QDebug>

#define MAX30102_I2C_ADDRESS 0x57

SensorMAX30102Widget::SensorMAX30102Widget(QWidget *parent)
    : QWidget(parent)
{
    initUI();
}

SensorMAX30102Widget::~SensorMAX30102Widget()
{
    qDebug() << "[LOG]: SensorMAX30102Widget Distructed";
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
    lineEdit_heart_rate_value = new QLineEdit;
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
    lineEdit_blood_oxygen_value = new QLineEdit;
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

    // Layout for sensor sampling rate adjustment
    QLabel *label_sampling_rate = new QLabel("Sampling Rate");
    comboBox_sampling_rate = new QComboBox(this);
    comboBox_sampling_rate->setMinimumSize(90, 0);
    QStringList samplingRates = { "50Hz", "100Hz", "200Hz", "400Hz", "800Hz", "1000Hz", "1600Hz", "3200Hz" };
    comboBox_sampling_rate->addItems(samplingRates);
    // ....
    // ....connect
    connect(comboBox_sampling_rate, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int index) {
        QVariantMap params;
        switch (index) {
            case 0: params["sampling rate"] = 0x00; break;  // 50Hz
            case 1: params["sampling rate"] = 0x01; break;  // 100Hz
            case 2: params["sampling rate"] = 0x02; break;  // 200Hz
            case 3: params["sampling rate"] = 0x03; break;  // 400Hz
            case 4: params["sampling rate"] = 0x04; break;  // 800Hz
            case 5: params["sampling rate"] = 0x05; break;  // 1000Hz
            case 6: params["sampling rate"] = 0x06; break;  // 1600Hz
            case 7: params["sampling rate"] = 0x07; break;  // 3200Hz
        }
        emit sig_SendControl("MAX30102", params);
    });

    // Layout for sensor red adjustment
    QLabel *label_led_red = new QLabel("LED Red");
    comboBox_led_red = new QComboBox(this);
    comboBox_led_red->setMinimumSize(90, 0);
    comboBox_led_red->addItem("30Hz");
    // ...
    // ....connect

    // Layout for sensor IR adjustment
    QLabel *label_led_ir = new QLabel("LED IR");
    comboBox_led_ir = new QComboBox(this);
    comboBox_led_ir->setMinimumSize(90, 0);
    comboBox_led_ir->addItem("30Hz");
    // ....
    // ....connect

    // Layout for sensor controller display
    QHBoxLayout *horizontalLayout_sensor_controller = new QHBoxLayout;
    horizontalLayout_sensor_controller->addWidget(label_sampling_rate);
    horizontalLayout_sensor_controller->addWidget(comboBox_sampling_rate);
    horizontalLayout_sensor_controller->addWidget(label_led_red);
    horizontalLayout_sensor_controller->addWidget(comboBox_led_red);
    horizontalLayout_sensor_controller->addWidget(label_led_ir);
    horizontalLayout_sensor_controller->addWidget(comboBox_led_ir);

    // Layout for sensor value displays
    QHBoxLayout *horizontalLayout_sensor_value_display = new QHBoxLayout;
    horizontalLayout_sensor_value_display->addLayout(verticalLayout_heart_rate);
    horizontalLayout_sensor_value_display->addLayout(verticalLayout_blood_oxygen);

    // Main layout
    QVBoxLayout *verticalLayout_max30102 = new QVBoxLayout;
    verticalLayout_max30102->addLayout(horizontalLayout_mode_config);
    verticalLayout_max30102->addLayout(horizontalLayout_sensor_value_display);
    verticalLayout_max30102->addLayout(horizontalLayout_sensor_controller);

    groupBox_sensor_max30102->setLayout(verticalLayout_max30102);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(groupBox_sensor_max30102);
    setLayout(mainLayout);
}

void SensorMAX30102Widget::on_comboBox_sampling_rate_currentIndexChanged(int index)
{
    qDebug() << "[LOG]: Sampling Rate Index" << index;
}

void SensorMAX30102Widget::on_comboBox_led_red_currentIndexChanged(int index)
{
    qDebug() << "[LOG]: LED RED Index" << index;
}

void SensorMAX30102Widget::on_comboBox_led_ir_currentIndexChanged(int index)
{
    qDebug() << "[LOG]: LED IR Index" << index;
}

// Slot function for heart rate slider value change
void SensorMAX30102Widget::on_slider_heart_rate_changed(int value)
{
    qDebug() << "[LOG]: Heart Rate:" << value;
}

// Slot function for blood oxygen slider value change
void SensorMAX30102Widget::on_slider_blood_oxygen_changed(int value)
{
    qDebug() << "[LOG]: Blood Oxygen:" << value << "%";
}

// Slot function to handle mode selection
void SensorMAX30102Widget::on_checkBox_mode_changed(bool checked)
{
    if (!checked) return;  // Ignore unchecked state

    if (checkBox_heart_rate_mode->isChecked()) {
        qDebug() << "Heart Rate Mode Selected";
    } else if (checkBox_blood_oxygen_mode->isChecked()) {
        qDebug() << "SpO2 Mode Selected";
    } else if (checkBox_mixed_mode->isChecked()) {
        qDebug() << "Mixed Mode Selected";
    }
}

void SensorMAX30102Widget::updateValues(const float &heartrate, const float &spo2)
{
    this->lineEdit_heart_rate_value->setText(QString::number(heartrate, 'f', 3));
    this->lineEdit_blood_oxygen_value->setText(QString::number(spo2, 'f', 3));
}
