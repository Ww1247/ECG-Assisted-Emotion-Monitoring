#ifndef CAMERACAPTURE_H
#define CAMERACAPTURE_H

#include <QObject>
#include "camera_driver.h"

class CameraCapture : public QObject
{
    Q_OBJECT

public:
    explicit CameraCapture(QObject *parent = nullptr);
    ~CameraCapture();

    void start(int cameraIndex = 0);  // Start the camera
    void stop();                      // Stop the camera
    void setFPS(int fps);             // Set FPS
    void setResolution(int width, int height); // Set resolution

signals:
    void frameReady(const QImage &frame);

private:
    CameraDriver *cameraDriver;
};

#endif // CAMERACAPTURE_H
