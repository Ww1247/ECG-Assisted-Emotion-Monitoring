#include "sensor_max30102.h"

SensorMAX30102Widget::SensorMAX30102Widget(QWidget *parent) : QWidget(parent) {
    initUI();
}

void SensorMAX30102Widget::initUI() {
    // Create MAX30102 sensor group box
    QGroupBox *groupBox_sensor_max30102 = new QGroupBox("MAX30102", this);
    groupBox_sensor_max30102->setGeometry(120, 120, 624, 152);

    // Sensor mode checkboxes
    QCheckBox *checkBox_heart_rate_mode = new QCheckBox("Heart Rate Mode");
    QCheckBox *checkBox_blood_oxygen_mode = new QCheckBox("SpO2 Mode");
    QCheckBox *checkBox_mixed_mode = new QCheckBox("Mixed Mode");
    checkBox_mixed_mode->setChecked(true);

    // Ensure only one mode can be selected at a time
    checkBox_heart_rate_mode->setAutoExclusive(true);
    checkBox_blood_oxygen_mode->setAutoExclusive(true);
    checkBox_mixed_mode->setAutoExclusive(true);

    // Layout for sensor mode selection
    QHBoxLayout *horizontalLayout_mode_config = new QHBoxLayout;
    horizontalLayout_mode_config->addWidget(checkBox_heart_rate_mode);
    horizontalLayout_mode_config->addWidget(checkBox_blood_oxygen_mode);
    horizontalLayout_mode_config->addWidget(checkBox_mixed_mode);
    horizontalLayout_mode_config->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

    // Heart rate display
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

    QSlider *slider_heart_rate = new QSlider(Qt::Horizontal);

    QVBoxLayout *verticalLayout_heart_rate = new QVBoxLayout;
    verticalLayout_heart_rate->addLayout(horizontalLayout_heart_rate);
    verticalLayout_heart_rate->addWidget(slider_heart_rate);

    // Blood oxygen display
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

    QSlider *slider_blood_oxygen = new QSlider(Qt::Horizontal);

    QVBoxLayout *verticalLayout_blood_oxygen = new QVBoxLayout;
    verticalLayout_blood_oxygen->addLayout(horizontalLayout_blood_oxygen);
    verticalLayout_blood_oxygen->addWidget(slider_blood_oxygen);

    // Layout for sensor value display
    QHBoxLayout *horizontalLayout_sensor_value_display = new QHBoxLayout;
    horizontalLayout_sensor_value_display->addLayout(verticalLayout_heart_rate);
    horizontalLayout_sensor_value_display->addLayout(verticalLayout_blood_oxygen);

    // Sensor configuration - Sampling rate
    QLabel *label_sampling_rate = new QLabel("Sampling Rate:");
    QComboBox *comboBox_sampling_rate = new QComboBox;
    comboBox_sampling_rate->addItems({"50Hz", "100Hz", "200Hz", "400Hz", "800Hz", "1600Hz"});

    QHBoxLayout *horizontalLayout_sampling_rate = new QHBoxLayout;
    horizontalLayout_sampling_rate->addWidget(label_sampling_rate);
    horizontalLayout_sampling_rate->addWidget(comboBox_sampling_rate);

    // Sensor configuration - LED Red
    QLabel *label_led_red = new QLabel("LED RED:");
    QComboBox *comboBox_led_red = new QComboBox;
    comboBox_led_red->addItems({"0%", "20%", "40%", "60%", "80%", "100%"});

    QHBoxLayout *horizontalLayout_led_red = new QHBoxLayout;
    horizontalLayout_led_red->addWidget(label_led_red);
    horizontalLayout_led_red->addWidget(comboBox_led_red);

    // Sensor configuration - LED IR
    QLabel *label_led_ir = new QLabel("LED IR:");
    QComboBox *comboBox_led_ir = new QComboBox;
    comboBox_led_ir->addItems({"0%", "20%", "40%", "60%", "80%", "100%"});

    QHBoxLayout *horizontalLayout_led_ir = new QHBoxLayout;
    horizontalLayout_led_ir->addWidget(label_led_ir);
    horizontalLayout_led_ir->addWidget(comboBox_led_ir);

    // Layout for sensor configuration
    QHBoxLayout *horizontalLayout_sensor_config = new QHBoxLayout;
    horizontalLayout_sensor_config->addLayout(horizontalLayout_sampling_rate);
    horizontalLayout_sensor_config->addLayout(horizontalLayout_led_red);
    horizontalLayout_sensor_config->addLayout(horizontalLayout_led_ir);
    horizontalLayout_sensor_config->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

    // Main layout
    QVBoxLayout *verticalLayout_max30102 = new QVBoxLayout;
    verticalLayout_max30102->addLayout(horizontalLayout_mode_config);
    verticalLayout_max30102->addLayout(horizontalLayout_sensor_value_display);
    verticalLayout_max30102->addLayout(horizontalLayout_sensor_config);

    groupBox_sensor_max30102->setLayout(verticalLayout_max30102);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(groupBox_sensor_max30102);
    setLayout(mainLayout);
}
