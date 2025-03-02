#include "camera_capture.h"
#include <QImage>
#include <QDebug>
#include <chrono>
#include <thread>

CameraCapture::CameraCapture(QObject *parent)
    : QObject(parent), running(false), frameWidth(640), frameHeight(480), targetFPS(30) {}

CameraCapture::~CameraCapture() {
    stop();
}

void CameraCapture::start(int cameraIndex) {
    if (running) return;
    running = true;
    captureThread = QThread::create([this, cameraIndex]() { captureLoop(); });
    captureThread->start();
}

void CameraCapture::stop() {
    running = false;
    if (captureThread && captureThread->isRunning()) {
        captureThread->quit();
        captureThread->wait();
    }
    cap.release();
}

void CameraCapture::setResolution(int width, int height) {
    QMutexLocker locker(&mutex);
    frameWidth = width;
    frameHeight = height;
    if (cap.isOpened()) {
        cap.set(cv::CAP_PROP_FRAME_WIDTH, width);
        cap.set(cv::CAP_PROP_FRAME_HEIGHT, height);
    }
}

void CameraCapture::setFPS(int fps) {
    QMutexLocker locker(&mutex);
    targetFPS = fps;
    if (cap.isOpened()) {
        cap.set(cv::CAP_PROP_FPS, fps);
    }
}

void CameraCapture::captureLoop() {
    cap.open(0);
    if (!cap.isOpened()) {
        qDebug() << "Failed to open camera";
        return;
    }
    cap.set(cv::CAP_PROP_FRAME_WIDTH, frameWidth);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, frameHeight);
    cap.set(cv::CAP_PROP_FPS, targetFPS);

    while (running) {
        auto startTime = std::chrono::steady_clock::now();
        
        cv::Mat frame;
        cap >> frame;
        if (frame.empty()) continue;
        
        cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
        QImage image(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
        emit frameReady(image.copy());
        
        auto endTime = std::chrono::steady_clock::now();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / targetFPS) - (endTime - startTime));
    }
}