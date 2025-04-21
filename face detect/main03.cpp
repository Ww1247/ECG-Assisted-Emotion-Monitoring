#include <opencv2/opencv.hpp>
#include <dlib/opencv.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <onnxruntime_cxx_api.h>
#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <chrono>
#include <thread>

using namespace std;
using namespace cv;

// MobileNet 情绪模型输出的常见 7 类
const vector<string> EMOTION_LABELS = {
    "Angry", "Disgust", "Fear", "Happy", "Sad", "Surprise", "Neutral"
};

// 三分类标签：0=Sad, 1=Happy, 2=Neutral
const vector<string> CATEGORY_LABELS = {"Sad", "Happy", "Neutral"};
const array<int, 7> CATEGORY_MAPPING = {0, 0, 0, 1, 0, 1, 2};

int main() {
    // 摄像头管道
    string pipeline =
        "libcamerasrc ! "
        "video/x-raw,format=RGB,width=320,height=240,framerate=30/1 ! "
        "videoconvert ! video/x-raw,format=BGR ! "
        "queue leaky=2 max-size-buffers=1 ! appsink drop=true sync=false";

    VideoCapture cap(pipeline, cv::CAP_GSTREAMER);
    if (!cap.isOpened()) {
        cerr << "❌ 无法打开摄像头" << endl;
        return -1;
    }

    // 初始化 ONNX Runtime
    Ort::Env env(ORT_LOGGING_LEVEL_WARNING, "emotion");
    Ort::SessionOptions session_options;
    session_options.SetIntraOpNumThreads(4);
    Ort::Session session(env, "mobilenetv2-7.onnx", session_options);

    Ort::AllocatorWithDefaultOptions allocator;
    Ort::AllocatedStringPtr input_name_ptr = session.GetInputNameAllocated(0, allocator);
    Ort::AllocatedStringPtr output_name_ptr = session.GetOutputNameAllocated(0, allocator);
    const char* input_names[] = { input_name_ptr.get() };
    const char* output_names[] = { output_name_ptr.get() };

    dlib::frontal_face_detector detector = dlib::get_frontal_face_detector();

    Mat frame;
    int frame_count = 0;
    const int DETECT_EVERY_N_FRAMES = 5;
    vector<Rect> cached_faces;

    // 用于统计前10秒内三类情绪次数
    vector<int> category_counts(3, 0);
    auto start_time = chrono::steady_clock::now();
    bool reported = false;

    while (cap.read(frame)) {
        if (frame.empty()) continue;

        // 缩小帧用于检测
        Mat small_frame;
        resize(frame, small_frame, Size(), 0.5, 0.5);

        // 每隔若干帧检测一次人脸
        if (frame_count % DETECT_EVERY_N_FRAMES == 0) {
            dlib::cv_image<dlib::bgr_pixel> dlib_img(small_frame);
            auto faces = detector(dlib_img);
            cached_faces.clear();
            for (auto& f : faces) {
                Rect r(Point(f.left()*2, f.top()*2), Point(f.right()*2, f.bottom()*2));
                r &= Rect(0, 0, frame.cols, frame.rows);
                cached_faces.push_back(r);
            }
        }

        // 多线程推理
        vector<Rect> faces = cached_faces;
        size_t n = faces.size();
        vector<int> max_idxs(n, 0);
        vector<thread> threads;

        for (size_t i = 0; i < n; ++i) {
            threads.emplace_back([&, i]() {
                // 克隆 ROI
                Mat face = frame(faces[i]).clone();
                resize(face, face, Size(224, 224));
                face.convertTo(face, CV_32F, 1.0/255.0);

                // BGR -> CHW
                vector<float> input_tensor;
                input_tensor.reserve(3*224*224);
                for (int c = 0; c < 3; ++c)
                    for (int y = 0; y < 224; ++y)
                        for (int x = 0; x < 224; ++x)
                            input_tensor.push_back(face.at<Vec3f>(y, x)[c]);

                array<int64_t,4> input_shape{1,3,224,224};
                auto value = Ort::Value::CreateTensor<float>(
                    Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault),
                    input_tensor.data(), input_tensor.size(), input_shape.data(), input_shape.size());

                auto outputs = session.Run(Ort::RunOptions{nullptr},
                                           input_names, &value, 1,
                                           output_names, 1);
                float* logits = outputs[0].GetTensorMutableData<float>();

                // softmax
                vector<float> probs(7);
                float sum = 0.0f;
                for (int j = 0; j < 7; ++j) sum += (probs[j] = exp(logits[j]));
                for (auto &p : probs) p /= sum;

                // 取最大索引
                max_idxs[i] = max_element(probs.begin(), probs.end()) - probs.begin();
            });
        }
        for (auto &t : threads) t.join();

        // 记录与绘制
        auto now = chrono::steady_clock::now();
        double elapsed = chrono::duration_cast<chrono::seconds>(now - start_time).count();
        for (size_t i = 0; i < n; ++i) {
            int idx = max_idxs[i];
            int cat = CATEGORY_MAPPING[idx];
            if (elapsed <= 10.0) {
                category_counts[cat]++;
            } else if (!reported) {
                // 输出10秒内最多的三类情绪
                int top_cat = max_element(category_counts.begin(), category_counts.end()) - category_counts.begin();
                cout << "在开始10秒内出现最多的情绪类别: " << CATEGORY_LABELS[top_cat] << endl;
                reported = true;
                // 如果需要退出，可取消下一行注释
                // break;
            }
            // 在帧上绘制分类标签
            putText(frame, CATEGORY_LABELS[cat], Point(faces[i].x, faces[i].y - 5),
                    FONT_HERSHEY_SIMPLEX, 0.6, Scalar(0, 255, 0), 2);
            rectangle(frame, faces[i], Scalar(0, 255, 0), 2);
        }

        imshow("MobileNet Emotion", frame);
        if (waitKey(1) == 'q') break;
        frame_count++;
    }

    return 0;
}
