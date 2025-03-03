#ifndef CAMERATHREAD_H
#define CAMERATHREAD_H

#include <QObject>
#include <QThread>
#include <QImage>
#include <QMutex>
#include <opencv2/opencv.hpp>

class CameraDriver : public QThread
{
    Q_OBJECT

public:
    CameraDriver(int cameraIndex = 0, int fps = 30, int width = 640, int height = 480, QObject *parent = nullptr);
    ~CameraDriver();

    void run() override;       // Thread execution function
    void stop();               // Stop the camera
    void setFPS(int newFPS);      // Set the frame rate
    void setResolution(int width, int newHeight); // Set the resolution

signals:
    void frameReady(const QImage &frame);  // Signal to send processed frames

private:
    cv::VideoCapture cap;
    int cameraIndex;
    int fps;
    int width;
    int height;
    bool running;
    QMutex mutex;
};

#endif // CAMERATHREAD_H
