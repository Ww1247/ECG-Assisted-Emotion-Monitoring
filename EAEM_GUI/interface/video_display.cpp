#include "video_display.h"

VideoDisplayWidget::VideoDisplayWidget(QWidget *parent) : QWidget(parent) {
    initUI();
}

void VideoDisplayWidget::initUI() {
    // Create Video Display group box
    QGroupBox *groupBox_video_display = new QGroupBox("Video Display", this);

    // Video display area
    QWidget *widget_video_display = new QWidget;
    widget_video_display->setMinimumSize(300, 300);

    // Resolution selection
    QLabel *label_resolution = new QLabel("Resolution:");
    QComboBox *comboBox_resolution = new QComboBox;
    comboBox_resolution->addItem("800x600");
    comboBox_resolution->addItem("1024x720");

    // Frame rate selection
    QLabel *label_video_fps = new QLabel("FPS:");
    QComboBox *comboBox_video_fps = new QComboBox;
    comboBox_video_fps->addItem("30");
    comboBox_video_fps->addItem("60");

    // Horizontal layout: resolution & frame rate
    QHBoxLayout *horizontalLayout_video_resolution = new QHBoxLayout;
    horizontalLayout_video_resolution->addWidget(label_resolution);
    horizontalLayout_video_resolution->addWidget(comboBox_resolution);

    QHBoxLayout *horizontalLayout_video_fps = new QHBoxLayout;
    horizontalLayout_video_fps->addWidget(label_video_fps);
    horizontalLayout_video_fps->addWidget(comboBox_video_fps);

    // Combine control buttons
    QHBoxLayout *horizontalLayout_video_control = new QHBoxLayout;
    horizontalLayout_video_control->addLayout(horizontalLayout_video_resolution);
    horizontalLayout_video_control->addLayout(horizontalLayout_video_fps);
    horizontalLayout_video_control->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

    // Vertical layout
    QVBoxLayout *verticalLayout_video_display = new QVBoxLayout;
    verticalLayout_video_display->addWidget(widget_video_display);
    verticalLayout_video_display->addLayout(horizontalLayout_video_control);

    // Set layout for the group box
    groupBox_video_display->setLayout(verticalLayout_video_display);

    // Set main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(groupBox_video_display);
    setLayout(mainLayout);
}
