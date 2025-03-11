#include "video_display.h"
#include <QDebug>

VideoDisplayWidget::VideoDisplayWidget(QWidget *parent)
    : QWidget(parent),
      camera(nullptr),
      processor(nullptr),
      cameraRunning(false)

{
    initUI();
}

VideoDisplayWidget::~VideoDisplayWidget()
{
//    cameraStop();
//    qDebug() << "Video Display Widget End";
}

void VideoDisplayWidget::initUI()
{
    // Create Video Display group box
    QGroupBox *groupBox_video_display = new QGroupBox("Video Display", this);

    // Frame for video display area
    QFrame *frame_video_display = new QFrame(groupBox_video_display);
    frame_video_display->setFrameStyle(QFrame::Sunken | QFrame::StyledPanel);
    frame_video_display->setMinimumSize(320, 320);

    // Video display area
    label_video_display = new QLabel(frame_video_display);
    label_video_display->setMinimumSize(300, 300);
    label_video_display->setAlignment(Qt::AlignCenter);
    label_video_display->setStyleSheet("background-color: black;");

    // Layout for video display
    QVBoxLayout *layout_video_display = new QVBoxLayout(frame_video_display);
    layout_video_display->addWidget(label_video_display);
    layout_video_display->setContentsMargins(0, 0, 0, 0);

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
    verticalLayout_video_display->addWidget(frame_video_display);
    verticalLayout_video_display->addLayout(horizontalLayout_video_control);

    // Set layout for the group box
    groupBox_video_display->setLayout(verticalLayout_video_display);

    // Set main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(groupBox_video_display);
    setLayout(mainLayout);
}

// Handle resolution change
void VideoDisplayWidget::on_comboBox_resolution_currentIndexChanged(int index)
{
//    if (index == 0) {
//        setResolution(800, 600);
//    } else if (index == 1) {
//        setResolution(1024, 720);
//    }
//    qDebug() << "Resolution changed to" << comboBox_resolution->currentText();
    int width, height;
    getSelectedResolution(width, height);
    if (camera) camera->setResolution(width, height);
}

// Handle FPS change
void VideoDisplayWidget::on_comboBox_video_fps_currentIndexChanged(int index)
{
//    int fps = (index == 0) ? 30 : 60;
//    setFPS(fps);
//    qDebug() << "FPS changed to" << fps;
    int fps = getSelectedFPS();
    if (camera) camera->setFPS(fps);
}

void VideoDisplayWidget::setFPS(int fps)
{
    if (camera) {
        camera->setFPS(fps);
    }
}

void VideoDisplayWidget::setResolution(int width, int height)
{
    if (!camera) return;
    camera->setResolution(width, height);
}

void VideoDisplayWidget::startCamera()
{
//    qDebug() << "Camera Started.";
//    if (camera) {
//        stopCamera();
//    }
//    camera->startCamera();
    if (cameraRunning) return;
    int width, height;
    getSelectedResolution(width, height);
    int fps = getSelectedFPS();

    qDebug() << "Starting camera with resolution" << width << "x" << height << "FPS:" << fps;

    if (!camera) {
        camera = new CameraDriver(0, fps, width, height, nullptr);
        processor = new VideoProcessor(nullptr);
        connect(camera, &CameraDriver::frameReady, processor, &VideoProcessor::processFrame);
        connect(processor, &VideoProcessor::processedFrameReady, this, &VideoDisplayWidget::updateFrame);
    }

    camera->setResolution(width, height);
    camera->setFPS(fps);
    camera->startCamera();

    cameraRunning = true;
}

void VideoDisplayWidget::stopCamera()
{
    if (!cameraRunning) return;

    qDebug() << "Stopping camera";

    if (camera) {
        camera->stopCamera();
        camera->deleteLater();
        processor->deleteLater();
        camera = nullptr;
        processor = nullptr;
    }

    cameraRunning = false;
}

int VideoDisplayWidget::getSelectedFPS() const
{
    return (comboBox_video_fps->currentIndex() == 0) ? 30 : 60;
}

void VideoDisplayWidget::getSelectedResolution(int &width, int &height) const
{
    if (comboBox_resolution->currentIndex() == 0) {
        width = 800;
        height = 600;
    } else {
        width = 1024;
        height = 720;
    }
}

void VideoDisplayWidget::toggleCamera(bool enable)
{
    if (enable)
        startCamera();
    else
        stopCamera();
}

// Update the video frame
void VideoDisplayWidget::updateFrame(const QImage &frame)
{
    label_video_display->setPixmap(QPixmap::fromImage(frame).scaled(label_video_display->size(),
                                                                    Qt::KeepAspectRatio,
                                                                    Qt::SmoothTransformation));
}
