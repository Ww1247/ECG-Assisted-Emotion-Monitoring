#include "camera_capture.h"

CameraCapture::CameraCapture(QObject *parent) : QObject(parent), cameraDriver(nullptr) {}

CameraCapture::~CameraCapture()
{
    stop();
}

void CameraCapture::start(int cameraIndex)
{
    if (cameraDriver) {
        stop();
    }
    cameraDriver = new CameraDriver(cameraIndex);
    connect(cameraDriver, &CameraDriver::frameReady, this, &CameraCapture::frameReady);
    cameraDriver->start();
}

void CameraCapture::stop()
{
    if (cameraDriver) {
        cameraDriver->stop();
        cameraDriver->deleteLater();
        cameraDriver = nullptr;
    }
}

void CameraCapture::setFPS(int fps)
{
    if (cameraDriver) {
        cameraDriver->setFPS(fps);
    }
}

void CameraCapture::setResolution(int width, int height)
{
    if (cameraDriver) {
        cameraDriver->setResolution(width, height);
    }
}
