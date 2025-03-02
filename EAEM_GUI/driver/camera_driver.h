#ifndef CAMERA_CAPTURE_H
#define CAMERA_CAPTURE_H

#include <QObject>
#include <QThread>
#include <QImage>
#include <QMutex>
#include <opencv2/opencv.hpp>

class CameraCapture : public QObject {
    Q_OBJECT
public:
    explicit CameraCapture(QObject *parent = nullptr);
    ~CameraCapture();

    void start(int cameraIndex = 0);
    void stop();
    void setResolution(int width, int height);
    void setFPS(int fps);

signals:
    void frameReady(const QImage &frame);

private:
    void captureLoop();

    QThread *captureThread;
    cv::VideoCapture cap;
    QMutex mutex;
    bool running;
    int frameWidth;
    int frameHeight;
    int targetFPS;
};

#endif // CAMERA_CAPTURE_H
