#include "camera_driver.h"
#include <QDebug>
#include <QDateTime>

bool CameraDriver::initialized = false;
cv::dnn::Net CameraDriver::net_;
cv::CascadeClassifier CameraDriver::faceCascade_;
std::vector<std::string> CameraDriver::emotionLabels_;

// Constructor
CameraDriver::CameraDriver(int cameraIndex, int width, int height, int fps, QObject *parent)
    : QObject(parent),
      cameraIndex(cameraIndex),
      width(width),
      height(height),
      fps(fps)
{}

// Destructor
CameraDriver::~CameraDriver()
{
    if (cap.isOpened()) {
        cap.release();
    }
}

bool CameraDriver::initialize()
{
    if (!cap.open(cameraIndex, cv::CAP_V4L2)) {
        qWarning() << "[Camera] Failed to open camera index" << cameraIndex;
        return false;
    }

    cap.set(cv::CAP_PROP_FRAME_WIDTH, width.load());
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, height.load());
    cap.set(cv::CAP_PROP_FPS, fps.load());
    cap.set(cv::CAP_PROP_BUFFERSIZE, 1); // Reduce lag

    if (!initialized) {
        qDebug() << "[CameraDriver] Loading face cascade and emotion model...";
        faceCascade_.load("haarcascade_frontalface_default.xml");
        net_ = cv::dnn::readNetFromONNX("emotion-ferplus-2.onnx");
        emotionLabels_ = {"Neutral", "Happy", "Surprise", "Sad", "Anger", "Disgust", "Fear", "Contempt"};
        initialized = true;
    }
    qDebug() << "[Camera] Initialized with" << width.load() << "x" << height.load() << "@" << fps.load() << "fps";
    return true;
}

bool CameraDriver::readOnce(SensorData &data)
{
    if (!cap.isOpened()) return false;

    cap.grab();
    cv::Mat frame;
    cap.retrieve(frame);
    if (frame.empty()) return false;
    cv::Mat bgrFrame = frame.clone();
    std::vector<cv::Rect> faces;
    faceCascade_.detectMultiScale(bgrFrame, faces, 1.3, 5);

    if (!faces.empty()) {
        for (const auto &face : faces) {
            double confidence = 0.0;
            QString emotion = predictEmotion(bgrFrame(face), confidence);
            data.values["emotion"] = emotion;
            data.values["confidence"] = confidence;
            cv::rectangle(bgrFrame, face, cv::Scalar(0, 255, 0), 2);
            std::string label = emotion.toStdString() + " (" + std::to_string(int(confidence * 100)) + "%)";
            int baseLine = 0;
            cv::Size textSize = cv::getTextSize(label, cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
            cv::Point textOrigin(face.x, face.y - 5);
            if (textOrigin.y < textSize.height) {
                textOrigin.y = face.y + face.height + textSize.height + 5;
            }
            cv::putText(bgrFrame, label, textOrigin,
                        cv::FONT_HERSHEY_SIMPLEX, 0.5,
                        cv::Scalar(0, 255, 255), 1);
        }
    } else {
        data.values["emotion"] = "No Face";
        data.values["confidence"] = 0.0;
    }
    cv::Mat rgbFrame;
    cv::cvtColor(bgrFrame, rgbFrame, cv::COLOR_BGR2RGB);
    QImage image(rgbFrame.data, rgbFrame.cols, rgbFrame.rows, rgbFrame.step, QImage::Format_RGB888);
    data.image = image.copy();

    data.sensor_name = "Camera";
    data.timestamp = QDateTime::currentDateTime();
    return true;
}


QString CameraDriver::predictEmotion(const cv::Mat &faceROI, double &confidenceOut)
{
    cv::Mat gray;
    cv::cvtColor(faceROI, gray, cv::COLOR_BGR2GRAY);
    cv::resize(gray, gray, cv::Size(64, 64));
    gray.convertTo(gray, CV_32F, 1.0 / 255.0);

    cv::Mat blob = cv::dnn::blobFromImage(gray);  // shape: [1,1,64,64]
    net_.setInput(blob);
    cv::Mat prob = net_.forward();

    cv::Point classIdPoint;
    minMaxLoc(prob.reshape(1, 1), nullptr, &confidenceOut, nullptr, &classIdPoint);
    int classId = classIdPoint.x;

    return (classId >= 0 && classId < emotionLabels_.size())
           ? QString::fromStdString(emotionLabels_[classId])
           : "Unknown";
}

void CameraDriver::setFPS(int newFPS)
{
    if (newFPS <= 0) return;
    fps.store(newFPS);
    if (cap.isOpened()) {
        cap.set(cv::CAP_PROP_FPS, fps.load());
    }
}

void CameraDriver::setResolution(int newWidth, int newHeight)
{
    if (newWidth <= 0 || newHeight <= 0) return;
    width.store(newWidth);
    height.store(newHeight);
    if (cap.isOpened()) {
        cap.set(cv::CAP_PROP_FRAME_WIDTH, width.load());
        cap.set(cv::CAP_PROP_FRAME_HEIGHT, height.load());
    }
}

void CameraDriver::applySetting(const QVariantMap &params)
{
    Q_UNUSED(params)
}
