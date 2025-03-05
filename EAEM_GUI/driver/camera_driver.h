#ifndef CAMERATHREAD_H
#define CAMERATHREAD_H

#include <QThread>
#include <QImage>
#include <opencv2/opencv.hpp>
#include <atomic>

class CameraDriver : public QThread
{
    Q_OBJECT

public:
    CameraDriver(int cameraIndex = 0, int fps = 30, int width = 640, int height = 480, QObject *parent = nullptr);
    ~CameraDriver();

    void stop();               // Stop the camera
    void setFPS(int newFPS);      // Set the frame rate
    void setResolution(int width, int newHeight); // Set the resolution

signals:
    void frameReady(const QImage &frame);  // Signal to send processed frames

private:
    cv::VideoCapture cap;
    int cameraIndex;
    std::atomic<int> fps;
    std::atomic<int> width;
    std::atomic<int> height;
    std::atomic<bool> running_;

protected:
    void run() override;       // Thread execution function

};

#endif // CAMERATHREAD_H
