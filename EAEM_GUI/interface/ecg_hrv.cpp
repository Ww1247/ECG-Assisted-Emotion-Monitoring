#include "ecg_hrv.h"

EcgHrvWidget::EcgHrvWidget(QWidget *parent) : QWidget(parent) {
    initUI();
}

void EcgHrvWidget::initUI() {
    // ECG Group Box
    QGroupBox *groupBox_ECG = new QGroupBox("ECG", this);

    // ECG Display Options
    QRadioButton *radioButton_ECG_display = new QRadioButton("ECG Display");
    QRadioButton *radioButton_blood_oxygen_display = new QRadioButton("Blood Oxygen");
    QRadioButton *radioButton_avg_heart_rate_display = new QRadioButton("Avg Heart Rate");
    radioButton_ECG_display->setChecked(true);

    QHBoxLayout *horizontalLayout_ECG_config = new QHBoxLayout;
    horizontalLayout_ECG_config->addWidget(radioButton_ECG_display);
    horizontalLayout_ECG_config->addWidget(radioButton_blood_oxygen_display);
    horizontalLayout_ECG_config->addWidget(radioButton_avg_heart_rate_display);
    horizontalLayout_ECG_config->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

    // ECG Display Frame
    QFrame *frame_ECG = new QFrame;
    frame_ECG->setMinimumSize(450, 150);
    frame_ECG->setFrameShape(QFrame::Box);

    QVBoxLayout *verticalLayout_ECG = new QVBoxLayout;
    verticalLayout_ECG->addLayout(horizontalLayout_ECG_config);
    verticalLayout_ECG->addWidget(frame_ECG);

    groupBox_ECG->setLayout(verticalLayout_ECG);

    // HRV Group Box
    QGroupBox *groupBox_HRV = new QGroupBox("HRV", this);

    // HRV Display Options
    QRadioButton *radioButton_HRV_RMSSD = new QRadioButton("RMSSD");
    QRadioButton *radioButton_HRV_SDNN = new QRadioButton("SDNN");
    QRadioButton *radioButton_HRV_LF_HF = new QRadioButton("LF/HF");
    radioButton_HRV_RMSSD->setChecked(true);

    QHBoxLayout *horizontalLayout_HRV_config = new QHBoxLayout;
    horizontalLayout_HRV_config->addWidget(radioButton_HRV_RMSSD);
    horizontalLayout_HRV_config->addWidget(radioButton_HRV_SDNN);
    horizontalLayout_HRV_config->addWidget(radioButton_HRV_LF_HF);
    horizontalLayout_HRV_config->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

    // HRV Display Frame
    QFrame *frame_HRV = new QFrame;
    frame_HRV->setMinimumSize(450, 150);
    frame_HRV->setFrameShape(QFrame::Box);

    QVBoxLayout *verticalLayout_HRV = new QVBoxLayout;
    verticalLayout_HRV->addLayout(horizontalLayout_HRV_config);
    verticalLayout_HRV->addWidget(frame_HRV);

    groupBox_HRV->setLayout(verticalLayout_HRV);

    // Main Layout
    QHBoxLayout *horizontalLayout_MAX30102 = new QHBoxLayout;
    horizontalLayout_MAX30102->addWidget(groupBox_ECG);
    horizontalLayout_MAX30102->addWidget(groupBox_HRV);

    setLayout(horizontalLayout_MAX30102);
}
