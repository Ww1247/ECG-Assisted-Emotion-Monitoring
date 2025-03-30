#include "camera_driver.h"
#include <QDebug>
#include <QDateTime>
#include <QThread>

// Constructor
CameraDriver::CameraDriver(int cameraIndex, int width, int height, int fps, QObject *parent)
    : QObject(parent),
      cameraIndex(cameraIndex),
      width(width),
      height(height),
      fps(fps)
{}

// Destructor
CameraDriver::~CameraDriver()
{
    if (cap.isOpened()) {
        cap.release();
    }
}

bool CameraDriver::initialize()
{
    if (!cap.open(cameraIndex, cv::CAP_V4L2)) {
        qWarning() << "[Camera] Failed to open camera index" << cameraIndex;
        return false;
    }

    cap.set(cv::CAP_PROP_FRAME_WIDTH, width.load());
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, height.load());
    cap.set(cv::CAP_PROP_FPS, fps.load());
    cap.set(cv::CAP_PROP_BUFFERSIZE, 1); // Reduce lag

    qDebug() << "[Camera] Initialized with" << width.load() << "x" << height.load() << "@" << fps.load() << "fps";
    return true;
}

bool CameraDriver::readOnce(SensorData &data)
{
//    QThread::currentThread()->setPriority(QThread::TimeCriticalPriority);
    if (!cap.isOpened()) return false;
    cap.grab();
    cv::Mat frame;
    cap.retrieve(frame);
    if (frame.empty()) return false;
    cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
    QImage image(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
    data.image = image.copy();
    data.name = "Camera";
    data.timestamp = QDateTime::currentDateTime();
    return true;
}

// Set frame rate
void CameraDriver::setFPS(int newFPS)
{
    if (newFPS <= 0) return;
    fps.store(newFPS);

    if (cap.isOpened()) {
        cap.set(cv::CAP_PROP_FPS, fps.load());
    }
}

// Set resolution
void CameraDriver::setResolution(int newWidth, int newHeight)
{
    if (newWidth <= 0 || newHeight <= 0) return;

    width.store(newWidth);
    height.store(newHeight);

    if (cap.isOpened()) {
        cap.set(cv::CAP_PROP_FRAME_WIDTH, width.load());
        cap.set(cv::CAP_PROP_FRAME_HEIGHT, height.load());
    }
}

void CameraDriver::applySetting(const QVariantMap &params)
{

}
