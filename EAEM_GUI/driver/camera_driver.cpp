#include "CameraThread.h"
#include <QDebug>
#include <QImage>
#include <QMutex>

// Constructor
CameraThread::CameraThread(int cameraIndex, int fps, int width, int height, QObject *parent)
    : QThread(parent), cameraIndex(cameraIndex), fps(fps), width(width), height(height), running(false) {}

// Destructor
CameraThread::~CameraThread()
{
    stop();
}

// Set frame rate
void CameraThread::setFPS(int fps)
{
    this->fps = fps;
}

// Set resolution
void CameraThread::setResolution(int width, int height)
{
    this->width = width;
    this->height = height;
}

// Run thread
void CameraThread::run()
{
    cap.open(cameraIndex);
    if (!cap.isOpened()) {
        qDebug() << "Error: Cannot open camera!";
        return;
    }
    cap.set(cv::CAP_PROP_FRAME_WIDTH, width);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, height);
    cap.set(cv::CAP_PROP_FPS, fps);

    running = true;
    while (running) {
        cv::Mat frame;
        cap >> frame;
        if (frame.empty()) {
            continue;
        }

        // Convert OpenCV Mat to QImage
        cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
        QImage image(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);

        // Emit signal to UI thread
        emit frameReady(image);

        QThread::msleep(1000 / fps);  // Control frame rate
    }
    cap.release();
}

// Stop thread
void CameraThread::stop()
{
    running = false;
    wait();
}
