#include "dashboard.h"

DashboardWidget::DashboardWidget(QWidget *parent) : QWidget(parent) {
    initUI();
}

void DashboardWidget::initUI() {
    // Create main group box
    QGroupBox *groupBox_Dashboard = new QGroupBox("Dashboard", this);
    groupBox_Dashboard->setGeometry(10, 20, 960, 65);

    // Create buttons
    pushButton_start_emotion_detection = new QPushButton("Start Emotion Detection");
    connect(pushButton_start_emotion_detection, &QPushButton::clicked, this, &DashboardWidget::sig_emotion_detection_start);

    pushButton_stop_emotion_detection = new QPushButton("Stop Emotion Detection");
    connect(pushButton_stop_emotion_detection, &QPushButton::clicked, this, &DashboardWidget::sig_emotion_detection_stop);

    // Create labels for system date and time
    QLabel *label_system_date = new QLabel();
    QLabel *label_system_time = new QLabel();
    label_system_date->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    label_system_time->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    updateSystemDateTime(label_system_date, label_system_time);  // Initialize with current values

    // Create layout for buttons
    QHBoxLayout *horizontalLayout_control_button = new QHBoxLayout;
    horizontalLayout_control_button->addWidget(pushButton_start_emotion_detection);
    horizontalLayout_control_button->addWidget(pushButton_stop_emotion_detection);

    // Create a vertical layout for date and time display
    QVBoxLayout *verticalLayout_time_display = new QVBoxLayout;
    verticalLayout_time_display->addWidget(label_system_date);
    verticalLayout_time_display->addWidget(label_system_time);

    // Create a spacer to push system time to the right
    QSpacerItem *horizontalSpacer_control_button = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    // Create main grid layout
    QGridLayout *gridLayout_16 = new QGridLayout;
    gridLayout_16->addLayout(horizontalLayout_control_button, 0, 0);
    gridLayout_16->addItem(horizontalSpacer_control_button, 0, 1);
    gridLayout_16->addLayout(verticalLayout_time_display, 0, 2, Qt::AlignRight); // Align right

    groupBox_Dashboard->setLayout(gridLayout_16);

    // Main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(groupBox_Dashboard);
    setLayout(mainLayout);

    // Timer to update date and time every second
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [label_system_date, label_system_time]() {
        updateSystemDateTime(label_system_date, label_system_time);
    });
    timer->start(1000);  // Update every second
}

// Function to update system date and time labels
void DashboardWidget::updateSystemDateTime(QLabel *dateLabel, QLabel *timeLabel) {
    QString currentDate = QDate::currentDate().toString("yyyy-MM-dd");
    QString currentTime = QTime::currentTime().toString("HH:mm:ss");

    dateLabel->setText("Date: " + currentDate);
    timeLabel->setText("Time: " + currentTime);
}
