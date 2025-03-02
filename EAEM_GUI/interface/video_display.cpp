#include "video_display.h"

VideoDisplayWidget::VideoDisplayWidget(QWidget *parent) : QWidget(parent) {
    initUI();
}

void VideoDisplayWidget::initUI() {
    // 创建 Video Display 组框
    QGroupBox *groupBox_video_display = new QGroupBox("Video Display", this);

    // 视频显示区域
    QWidget *widget_video_display = new QWidget;
    widget_video_display->setMinimumSize(300, 300);

    // 分辨率选择
    QLabel *label_resolution = new QLabel("Resolution:");
    QComboBox *comboBox_resolution = new QComboBox;
    comboBox_resolution->addItem("800x600");
    comboBox_resolution->addItem("1024x720");

    // 帧率选择
    QLabel *label_video_fps = new QLabel("FPS:");
    QComboBox *comboBox_video_fps = new QComboBox;
    comboBox_video_fps->addItem("30");
    comboBox_video_fps->addItem("60");

    // 水平布局：分辨率 & 帧率
    QHBoxLayout *horizontalLayout_video_resolution = new QHBoxLayout;
    horizontalLayout_video_resolution->addWidget(label_resolution);
    horizontalLayout_video_resolution->addWidget(comboBox_resolution);

    QHBoxLayout *horizontalLayout_video_fps = new QHBoxLayout;
    horizontalLayout_video_fps->addWidget(label_video_fps);
    horizontalLayout_video_fps->addWidget(comboBox_video_fps);

    // 组合控制按钮
    QHBoxLayout *horizontalLayout_video_control = new QHBoxLayout;
    horizontalLayout_video_control->addLayout(horizontalLayout_video_resolution);
    horizontalLayout_video_control->addLayout(horizontalLayout_video_fps);

    // 纵向布局
    QVBoxLayout *verticalLayout_video_display = new QVBoxLayout;
    verticalLayout_video_display->addWidget(widget_video_display);
    verticalLayout_video_display->addLayout(horizontalLayout_video_control);

    // 组框设置布局
    groupBox_video_display->setLayout(verticalLayout_video_display);

    // 设置主布局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(groupBox_video_display);
    setLayout(mainLayout);
}
