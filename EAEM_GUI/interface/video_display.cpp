#include "video_display.h"
#include <QDebug>

VideoDisplayWidget::VideoDisplayWidget(QWidget *parent) : QWidget(parent) {
    camera_capture = new CameraCapture(this);
    connect(camera_capture, &CameraCapture::frameReady, this, &VideoDisplayWidget::updateFrame);
    camera_capture->start();
    initUI();
}

VideoDisplayWidget::~VideoDisplayWidget()
{
    camera_capture->stop();
    qDebug() << "Video Display Widget End";
}

void VideoDisplayWidget::initUI() {
    // Create Video Display group box
    QGroupBox *groupBox_video_display = new QGroupBox("Video Display", this);
    groupBox_video_display->setContentsMargins(9, 9, 9, 9);

    // Video display area
    widget_video_display = new QWidget(this);
    widget_video_display->setMinimumSize(300, 300);

    // Resolution selection
    QLabel *label_resolution = new QLabel("Resolution:");
    comboBox_resolution = new QComboBox(this);
    comboBox_resolution->addItem("800x600");
    comboBox_resolution->addItem("1024x720");
    connect(comboBox_resolution, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &VideoDisplayWidget::on_comboBox_resolution_currentIndexChanged);

    // Frame rate selection
    QLabel *label_video_fps = new QLabel("FPS:");
    QComboBox *comboBox_video_fps = new QComboBox(this);
    comboBox_video_fps->addItem("30");
    comboBox_video_fps->addItem("60");
    connect(comboBox_video_fps, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &VideoDisplayWidget::on_comboBox_video_fps_currentIndexChanged);

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

// Update the video frame
void VideoDisplayWidget::updateFrame(const QImage &frame)
{
    image = frame;
    update(); // Trigger repaint
}

// Render the video frame onto the display widget
void VideoDisplayWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    if (!image.isNull()) {
        painter.drawImage(widget_video_display->geometry(), image);
    }
}

// Handle resolution change
void VideoDisplayWidget::on_comboBox_resolution_currentIndexChanged(int index)
{
    if (index == 0) {
        camera_capture->setResolution(800, 600);
    } else if (index == 1) {
        camera_capture->setResolution(1024, 720);
    }
    qDebug() << "Resolution changed to" << comboBox_resolution->currentText();
}

// Handle FPS change
void VideoDisplayWidget::on_comboBox_video_fps_currentIndexChanged(int index)
{
    int fps = (index == 0) ? 30 : 60;
    camera_capture->setFPS(fps);
    qDebug() << "FPS changed to" << fps;
}
