#include "camera_driver.h"

#include <QDebug>
#include <QImage>
#include <QMutex>

// Constructor
CameraDriver::CameraDriver(int cameraIndex, int fps, int width, int height, QObject *parent)
    : QThread(parent),
      cameraIndex(cameraIndex),
      fps(fps),
      width(width),
      height(height),
      running(false)
{}

// Destructor
CameraDriver::~CameraDriver()
{
    stop();
}

// Set frame rate
void CameraDriver::setFPS(int newFPS)
{
    QMutexLocker locker(&mutex); // Ensure thread safety
    newFPS = fps;
    if (cap.isOpened()) {
        cap.set(cv::CAP_PROP_FPS, newFPS);
        qDebug() << "Updated FPS to" << newFPS;
    }
}

// Set resolution
void CameraDriver::setResolution(int newWidth, int newHeight)
{
    QMutexLocker locker(&mutex); // Ensure thread safety
    width = newWidth;
    height = newHeight;
    if (cap.isOpened()) {
        cap.set(cv::CAP_PROP_FRAME_WIDTH, width);
        cap.set(cv::CAP_PROP_FRAME_HEIGHT, height);
        qDebug() << "Updated resolution to" << width << "x" << height;
    }
}

// Run thread
void CameraDriver::run()
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
void CameraDriver::stop()
{
    running = false;
    wait();
}
