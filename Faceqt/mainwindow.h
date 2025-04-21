#pragma once

#include <QMainWindow>
#include <QTimer>
#include <QElapsedTimer>
#include <QImage>
#include <QLabel>
#include <opencv2/opencv.hpp>
#include <dlib/image_processing/frontal_face_detector.h>
#include <onnxruntime_cxx_api.h>

class FrameGrabber;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void toggleDetection();
    void handleFrame(cv::Mat frame);

private:
    QLabel* resultLabel;
    Ui::MainWindow *ui;
    FrameGrabber* grabber;
    QElapsedTimer* uiTimer;

    dlib::frontal_face_detector detector;
    Ort::Env ort_env;
    Ort::Session ort_session;
    const char* input_name;
    const char* output_name;

    std::chrono::steady_clock::time_point start_time;
    std::vector<int> category_counts;
    std::vector<cv::Rect> cached_faces;

    bool isDetecting;
    int frame_count;
    bool reported;

    std::vector<int> predictEmotions(const cv::Mat& frame, const std::vector<cv::Rect>& faces);
    void drawPredictions(cv::Mat& frame, const std::vector<int>& emotion_indices);
};
