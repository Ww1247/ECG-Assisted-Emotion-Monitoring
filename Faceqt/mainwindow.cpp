#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <dlib/opencv.h>
#include <QThread>
#include <QElapsedTimer>
#include <QLabel>
#include <QDebug>
#include <thread>
#include <vector>
#include <algorithm>

using namespace std;
using namespace cv;

const vector<string> EMOTION_LABELS = {"Angry", "Disgust", "Fear", "Happy", "Sad", "Surprise", "Neutral"};
const vector<string> CATEGORY_LABELS = {"Sad", "Happy", "Neutral"};
const array<int, 7> CATEGORY_MAPPING = {0, 0, 0, 1, 0, 1, 2};

class FrameGrabber : public QThread {
public:
    cv::VideoCapture cap;
    std::function<void(cv::Mat)> onNewFrame;
    std::string pipeline;
    bool stopFlag = false;

    void run() override {
        cap.open(pipeline, cv::CAP_GSTREAMER);
        if (!cap.isOpened()) {
            qDebug() << "❌ 无法打开摄像头";
            return;
        }
        cv::Mat frame;
        while (!stopFlag) {
            cap.read(frame);
            if (!frame.empty() && onNewFrame) onNewFrame(frame.clone());
            QThread::msleep(30);
        }
    }

    void stop() {
        stopFlag = true;
        wait();
        cap.release();
    }
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow),
      ort_env(ORT_LOGGING_LEVEL_WARNING, "emotion"),
      ort_session(ort_env, "mobilenetv2-7.onnx", Ort::SessionOptions{}),
      isDetecting(false), frame_count(0), reported(false)
{
    ui->setupUi(this);

    resultLabel = new QLabel(this);
    resultLabel->setStyleSheet("font-size: 16px; color: blue;");
    resultLabel->setAlignment(Qt::AlignCenter);
    ui->verticalLayout->addWidget(resultLabel);

    grabber = new FrameGrabber();
    grabber->pipeline =
        "libcamerasrc ! video/x-raw,format=RGB,width=640,height=480,framerate=30/1 ! "
        "videoconvert ! video/x-raw,format=BGR ! appsink";

    detector = dlib::get_frontal_face_detector();
    Ort::AllocatorWithDefaultOptions allocator;
    input_name = ort_session.GetInputNameAllocated(0, allocator).release();
    output_name = ort_session.GetOutputNameAllocated(0, allocator).release();

    connect(grabber, &QThread::finished, grabber, &QObject::deleteLater);
    grabber->onNewFrame = [this](cv::Mat frame) { this->handleFrame(frame); };

    uiTimer = new QElapsedTimer();
    uiTimer->start();

    connect(ui->startButton, &QPushButton::clicked, this, &MainWindow::toggleDetection);
}

MainWindow::~MainWindow() {
    if (grabber->isRunning()) grabber->stop();
    delete ui;
}

void MainWindow::toggleDetection() {
    isDetecting = !isDetecting;
    if (isDetecting) {
        start_time = chrono::steady_clock::now();
        category_counts.assign(3, 0);
        reported = false;
        frame_count = 0;
        resultLabel->clear();
        grabber->start();
        ui->startButton->setText("停止检测");
    } else {
        grabber->stop();
        ui->startButton->setText("开始检测");
    }
}

void MainWindow::handleFrame(cv::Mat frame) {
    if (!isDetecting) return;
    frame_count++;

    cv::Mat small_frame;
    cv::resize(frame, small_frame, Size(), 0.5, 0.5);

    if (frame_count % 5 == 0) {
        cached_faces.clear();
        dlib::cv_image<dlib::bgr_pixel> dlib_img(small_frame);
        for (auto& f : detector(dlib_img)) {
            Rect r(Point(f.left()*2, f.top()*2), Point(f.right()*2, f.bottom()*2));
            r &= Rect(0, 0, frame.cols, frame.rows);
            cached_faces.push_back(r);
        }
        vector<int> emotion_indices = predictEmotions(frame, cached_faces);
        drawPredictions(frame, emotion_indices);
    }

    if (uiTimer->elapsed() > 100) {
        QImage image(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
        if (!image.isNull()) {
            ui->videoLabel->setPixmap(QPixmap::fromImage(image));
            ui->videoLabel->setScaledContents(true);
        }
        uiTimer->restart();
    }
}

vector<int> MainWindow::predictEmotions(const Mat& frame, const vector<Rect>& faces) {
    size_t n = faces.size();
    std::vector<int> results(n, 0);
    for (size_t i = 0; i < n; ++i) {
        Mat face = frame(faces[i]).clone();
        cv::resize(face, face, Size(224, 224));
        face.convertTo(face, CV_32F, 1.0 / 255.0);

        vector<float> input_tensor(3 * 224 * 224);
        for (int c = 0; c < 3; ++c)
            for (int y = 0; y < 224; ++y)
                for (int x = 0; x < 224; ++x)
                    input_tensor[c * 224 * 224 + y * 224 + x] = face.at<Vec3f>(y, x)[c];

        array<int64_t, 4> input_shape{1, 3, 224, 224};
        auto input_tensor_value = Ort::Value::CreateTensor<float>(
            Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault),
            input_tensor.data(), input_tensor.size(), input_shape.data(), input_shape.size());

        auto output = ort_session.Run(Ort::RunOptions{nullptr},
                                      &input_name, &input_tensor_value, 1,
                                      &output_name, 1);
        float* logits = output[0].GetTensorMutableData<float>();

        vector<float> probs(7);
        float sum = 0.0f;
        for (int j = 0; j < 7; ++j) sum += (probs[j] = exp(logits[j]));
        for (auto& p : probs) p /= sum;

        results[i] = max_element(probs.begin(), probs.end()) - probs.begin();
    }

    auto now = chrono::steady_clock::now();
    double elapsed = chrono::duration_cast<chrono::seconds>(now - start_time).count();
    for (int idx : results) {
        int cat = CATEGORY_MAPPING[idx];
        if (elapsed <= 1.0)
            category_counts[cat]++;
    }
    if (!reported && chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now() - start_time).count() > 10.0) {
        int top = max_element(category_counts.begin(), category_counts.end()) - category_counts.begin();
        QString resultText = "10秒最多情绪: " + QString::fromStdString(CATEGORY_LABELS[top]);
        resultLabel->setText(resultText);
        reported = true;
    }
    return results;
}

void MainWindow::drawPredictions(Mat& frame, const vector<int>& emotion_indices) {
    for (size_t i = 0; i < cached_faces.size(); ++i) {
        int cat = CATEGORY_MAPPING[emotion_indices[i]];
        putText(frame, CATEGORY_LABELS[cat], Point(cached_faces[i].x, cached_faces[i].y - 5),
                FONT_HERSHEY_SIMPLEX, 0.6, Scalar(0, 255, 0), 2);
        rectangle(frame, cached_faces[i], Scalar(0, 255, 0), 2);
    }
}
