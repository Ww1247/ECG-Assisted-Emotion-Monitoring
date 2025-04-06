#ifndef CAMERATHREAD_H
#define CAMERATHREAD_H

#pragma once

#include <QObject>
#include <QImage>
#include <atomic>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include "sensor_interface.h"
#include "sensor_data.h"

/**
 * @brief CameraDriver is a sensor class for capturing frames from a camera device.
 *
 * It provides real-time frame acquisition along with integrated face detection
 * and emotion recognition using OpenCV DNN and Haar cascades.
 */
class CameraDriver : public QObject, public SensorInterface
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a CameraDriver instance.
     * @param cameraIndex Index of the camera device (default is 0).
     * @param width Desired frame width.
     * @param height Desired frame height.
     * @param fps Desired frames per second.
     * @param parent Parent QObject.
     */
    CameraDriver(int cameraIndex = 0,
                 int width = 640,
                 int height = 480,
                 int fps = 60,
                 QObject *parent = nullptr);

    /**
     * @brief Destructor for CameraDriver.
     */
    ~CameraDriver();

    /**
     * @brief Initializes the camera and emotion detection models.
     * @return true if initialization succeeded, false otherwise.
     */
    bool initialize() override;

    /**
     * @brief Captures a single frame from the camera and performs face/emotion analysis.
     * @param data Reference to a SensorData structure to store the result.
     * @return true if frame was captured and processed successfully.
     */
    bool readOnce(SensorData &data) override;

    /**
     * @brief Applies parameter settings to the camera (currently unused).
     * @param params Key-value pairs of parameters.
     */
    void applySetting(const QVariantMap &params) override;

    /**
     * @brief Sets the frame rate of the camera.
     * @param newFPS Desired FPS value.
     */
    void setFPS(int newFPS);                 // Set the frame rate

    /**
     * @brief Sets the resolution of the camera.
     * @param width Desired frame width.
     * @param height Desired frame height.
     */
    void setResolution(int width, int height); // Set the resolution

private:
    int cameraIndex;                      ///< Index of the camera device.
    std::atomic<int> fps, width, height;  ///< Desired frame parameters.
    cv::VideoCapture cap;                 ///< OpenCV video capture object.

    /**
     * @brief Predicts the emotion label of a given face region.
     * @param faceROI Region of interest containing the face (BGR format).
     * @param confidenceOut Output: confidence score of the prediction.
     * @return Predicted emotion label as a QString.
     */
    QString predictEmotion(const cv::Mat &faceROI, double &confidenceOut);

    // Static members to share model and classifier across all instances
    static bool initialized;                         ///< Whether model has been initialized.
    static cv::dnn::Net net_;                        ///< OpenCV DNN network for emotion recognition.
    static cv::CascadeClassifier faceCascade_;       ///< Haar cascade classifier for face detection.
    static std::vector<std::string> emotionLabels_;  ///< List of emotion class labels.
};

#endif // CAMERATHREAD_H
