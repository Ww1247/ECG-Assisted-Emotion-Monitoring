#include "camera_driver.h"
#include <QDebug>

// Constructor
CameraDriver::CameraDriver(int cameraIndex, int fps, int width, int height, QObject *parent)
    : QThread(parent),
      cameraIndex(cameraIndex),
      fps(fps),
      width(width),
      height(height),
      running_(false)
{}

// Destructor
CameraDriver::~CameraDriver()
{
    stopCamera();
}

// Stop thread
void CameraDriver::stopCamera()
{
    qDebug() << "[LOG]: Camera thread stopped";
    running_.store(false, std::memory_order_relaxed);
    wait();
    emit sig_cameraStopped();
}

// Run in thread
void CameraDriver::run()
{
    cap.open(cameraIndex, cv::CAP_V4L2); // Turn on camera

    if (!cap.isOpened()) {
        qDebug() << "[Error]: Cannot open camera";
        return;
    }
    cap.set(cv::CAP_PROP_FRAME_WIDTH, width.load());
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, height.load());
    cap.set(cv::CAP_PROP_FPS, fps.load());
    cap.set(cv::CAP_PROP_BUFFERSIZE, 1); // reduce lag

    running_.store(true, std::memory_order_relaxed);

    while (running_.load(std::memory_order_relaxed)) {
        cv::Mat frame;
        cap >> frame;
        if (frame.empty()) {
            qDebug() << "[WARNING]: Empty frame received!";
            QThread::msleep(500);
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

// Set frame rate
void CameraDriver::setFPS(int newFPS)
{
    if (newFPS <= 0) return;

    fps.store(newFPS, std::memory_order_relaxed);
    if (cap.isOpened()) {
        if (!cap.set(cv::CAP_PROP_FPS, fps.load())) {
            qDebug() << "[WARNING]: CAP_PROP_FPS not supported by this camera.";
        } else {
            qDebug() << "[USER]: Updated FPS to" << fps.load();
        }
    }
}

// Set resolution
void CameraDriver::setResolution(int newWidth, int newHeight)
{
    if (newWidth <= 0 || newHeight <= 0) return;

    width.store(newWidth, std::memory_order_relaxed);
    height.store(newHeight, std::memory_order_relaxed);

    if (cap.isOpened()) {
        qDebug() << "[LOG]: Restarting camera with new resolution: " << width.load() << "x" << height.load();

        cap.release(); // Release the current camera completely
        QThread::msleep(100); // Wait 100ms for the camera to reinitialize
        cap.open(cameraIndex, cv::CAP_V4L2); // Turn the camera back on

        if (!cap.isOpened()) {
            qDebug() << "[Error]: Could not reopen camera after resolution change!";
            return;
        }

        cap.set(cv::CAP_PROP_FRAME_WIDTH, width.load());
        cap.set(cv::CAP_PROP_FRAME_HEIGHT, height.load());

        QThread::msleep(500); // Wait another 500ms to make sure the camera is stable
    }
}
