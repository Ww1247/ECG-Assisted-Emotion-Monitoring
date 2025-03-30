#ifndef CAMERATHREAD_H
#define CAMERATHREAD_H

#pragma once
#include <QObject>
#include <QImage>
#include <atomic>
#include <opencv2/opencv.hpp>
#include "sensor_interface.h"
#include "sensor_data.h"

class CameraDriver : public QObject, public SensorInterface
{
    Q_OBJECT

public:
    CameraDriver(int cameraIndex = 0,
                 int width = 640,
                 int height = 480,
                 int fps = 60,
                 QObject *parent = nullptr);
    ~CameraDriver();

    bool initialize() override;
    bool readOnce(SensorData &data) override;
    void applySetting(const QVariantMap &params) override;

    void setFPS(int newFPS);      // Set the frame rate
    void setResolution(int width, int newHeight); // Set the resolution

private:
    int cameraIndex;
    std::atomic<int> fps, width, height;
    cv::VideoCapture cap;
};

#endif // CAMERATHREAD_H
