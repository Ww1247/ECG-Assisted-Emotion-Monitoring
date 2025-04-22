#pragma once

#include <QMainWindow>
#include <QTimer>
#include <QLabel>
#include <QElapsedTimer>
#include <QChartView>
#include <QLineSeries>
#include <QValueAxis>
#include <QThread>
#include <QVector>
#include <QDateTime>
#include <opencv2/opencv.hpp>
#include <dlib/image_processing/frontal_face_detector.h>
#include <onnxruntime_cxx_api.h>

QT_CHARTS_USE_NAMESPACE

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class FrameGrabber;
class HeartWorker;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void toggleDetection();
    void handleCameraFrame(cv::Mat frame);
    void updateBPM(float bpm);
    void appendIRSample(int value);

private:
    Ui::MainWindow *ui;
    QTimer* timer;
    QElapsedTimer* uiTimer;
    QLabel* resultLabel;

    FrameGrabber* grabber;
    HeartWorker* worker;
    QThread* heartThread;

    dlib::frontal_face_detector detector;
    Ort::Env ort_env;
    Ort::Session ort_session;
    const char* input_name;
    const char* output_name;
    std::vector<cv::Rect> cached_faces;
    bool isDetecting;
    int frame_count;
    bool reported;
    std::chrono::steady_clock::time_point start_time;
    std::vector<int> category_counts;

    std::vector<int> predictEmotions(const cv::Mat& frame, const std::vector<cv::Rect>& faces);
    void drawPredictions(cv::Mat& frame, const std::vector<int>& emotion_indices);

    int i2cFile;
    QLineSeries* series;
    QChart* chart;
    QChartView* chartView;
    QValueAxis *axisX, *axisY;
    int xValue = 0;

    QLabel *rateLabel, *statusLabel, *maxRateLabel;
    QVector<QPair<qint64, float>> bpmHistory;
    qint64 startTime;
    bool tenSecondsProcessed;

    void initSensor();
    int i2c_write(int file, uint8_t reg, uint8_t value);
};
