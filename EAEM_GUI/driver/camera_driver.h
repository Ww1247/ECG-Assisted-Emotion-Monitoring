#ifndef CAMERATHREAD_H
#define CAMERATHREAD_H

#include <QObject>
#include <QThread>
#include <QImage>
#include <atomic>
#include <opencv2/opencv.hpp>

class CameraDriver : public QThread
{
    Q_OBJECT

protected:
    void run() override;

public:
    CameraDriver(int cameraIndex = 0,
                 int fps = 30,
                 int width = 640,
                 int height = 480,
                 QObject *parent = nullptr);
    ~CameraDriver();

    void stopCamera();
    void setFPS(int newFPS);      // Set the frame rate
    void setResolution(int width, int newHeight); // Set the resolution

private:
    int cameraIndex;
    std::atomic<int> fps;
    std::atomic<int> width;
    std::atomic<int> height;
    std::atomic<bool> running_;

    cv::VideoCapture cap;
    QThread *cameraThread_;

private slots:


signals:
    void frameReady(const QImage &frame);  // Signal to send processed frames
    void sig_cameraStopped();
};

#endif // CAMERATHREAD_H
