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

public:
    CameraDriver(int cameraIndex,
                 int fps,
                 int width,
                 int height,
                 QObject *parent = nullptr);
    ~CameraDriver();

    void startCamera();
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
    void process();

signals:
    void frameReady(const QImage &frame);  // Signal to send processed frames
    void stopped();
};

#endif // CAMERATHREAD_H
