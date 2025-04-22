<div align="center">
    <img src="Documents/pictures/EAEM-Logo.png"/ width=300>
    <p>
      <i align="center">An ECG-Assisted Emotion Monitoring System that captures facial expressions via a camera and extracts micro-expression features using image recognition. By integrating ECG signals, the system aims to develop an ECG-assisted emotion monitoring system that captures facial expressions via a camera and extracts micro-expression features using image recognition. By integrating ECG signals,the system aims for precise emotion assessment, enhancing mental health monitoring and awareness.🫀</i>
    </p>
    <div id="badges">
     <a href="operation system"><img src="https://img.shields.io/badge/Debian-A81D33?logo=debian&logoColor=fff"/></a>
     <a href="programming language"><img src="https://img.shields.io/badge/C++-%2300599C.svg?logo=c%2B%2B&logoColor=white"/></a>
     <a href="GitHub License"><img src="https://img.shields.io/badge/license-MIT-blue.svg"/></a>
     <a href="https://github.com/Ww1247/ECG-Assisted-Emotion-Monitoring/issues"><img src="https://img.shields.io/github/issues/Ww1247/ECG-Assisted-Emotion-Monitoring.svg"/></a>
     <a href="https://github.com/Ww1247/ECG-Assisted-Emotion-Monitoring"><img src="https://img.shields.io/github/stars/Ww1247/ECG-Assisted-Emotion-Monitoring.svg?style=social&label=Star"/></a>
     <a href="https://github.com/Ww1247/ECG-Assisted-Emotion-Monitoring"><img src="https://img.shields.io/github/forks/Ww1247/ECG-Assisted-Emotion-Monitoring?style=social&label=Fork"/></a>
    </div> 
       <br>
       <a href="https://youtube.com/shorts/V1aN6sqYCZ0?si=-a67i9UdDjk2yYoA"><img src="https://img.shields.io/badge/Youtube-%23E4405F.svg?logo=Youtube&logoColor=white"/></a>
       <br>
       <br>

[Hardware Installation](#installation) •
[Software Deployment](#examples) •
[Documentation](#documentation) •
[Contributors](#contributions)

</div>

# Project Overview

The **ECG Assisted Emotion Monitoring System** is a real-time health and wellness monitoring system designed to enhance user safety and performance during physical activities. Built on Raspberry Pi 5, this system integrates a Raspberry Pi Camera and a MAX30102 sensor to assess a user’s physiological and emotional state during exercise.

By leveraging computer vision algorithms, the system detects positive expressions (e.g., smiles, focused engagement) and negative expressions (e.g., grimaces, signs of strain or fatigue), while simultaneously tracking heart rate variability. If the system identifies negative expressions, abnormal heart rate trends, or unsafe physiological thresholds, it triggers customizable alerts to encourage the user to adjust intensity, hydrate, or pause for recovery.

**Key Goals**:

- Prevent overexertion during workouts.
- Foster emotional and physiological awareness.
- Optimise exercise efficiency through real-time feedback.

Here is the Raspberry Pi cross-compilation instructions: ([English](https://github.com/Ww1247/ECG-Assisted-Emotion-Monitoring/wiki/ECG-Assisted-Emotion-Monitoring%E2%80%90en) | [中文](https://github.com/Ww1247/ECG-Assisted-Emotion-Monitoring/wiki/ECG-Assisted-Emotion-Monitoring%E2%80%90zh))

## Key Innovations

* First-of-its-kind **Real-time fusion of facial expressions and ECG signals** on Raspberry Pi 5, overcoming single-sensor limitations.
* **Hybrid Model Architecture:**: Achieving an 8% improvement in accuracy at 12MB model size.
* **GStreamer Hardware-Accelerated Pipeline:** 35ms end-to-end video latency, 3× faster than CPU-only methods.
 
# Features

## 1. Emotion Monitoring

- **Hardware**: Raspberry Pi Camera (CSI interface).
- **Workflow**:
  - Captures real-time facial video via GStreamer pipeline.
  - Uses **dlib** for face detection and **ONNX Runtime** to classify emotions (happy/sad/neutral).
  - Displays emotion probabilities (softmax-normalized scores) on video frames with bounding boxes and labels.

## 2. Heart Rate Monitoring

- **Hardware**: MAX30102 sensor (I2C interface).
- **Workflow**:
  - Collects IR data for heart rate calculation.
  - Applies low-pass filtering and peak detection to derive heart rate (BPM).
  - Visualizes real-time ECG waveforms and BPM values using **Qt Charts** in a GUI.

## 3. Alert Scenarios

| Scenario                     | Example Prompt          | Action Required          |  
|------------------------------|-------------------------|--------------------------|  
| Positive + Normal HR          | "You're doing fantastic! Keep going! 🚀" | Maintain current intensity        |  
| Positive + Highl HR          | "Great effort! Slow down a bit to stay steady.  💪" | Reduce intensity/hydrate |  
| Neutral + Normal HR          | "You're on track! Keep going at this pace. 👌"  | Adjust effort if needed        |  
| Neutral + High HR          | "You're working hard! Ease off a bit. ⚠️"        | Immediately reduce intensity      |  
| Negative + Normal HR  | "Take a quick break to reset. You're doing fine!  😌"| Pause briefly/adjust posture       |  
| Negative + High HR            | "Alert: Let's pause now. Focus on recovery first. 🛑"  | Immediately stop and recover   |

# Hardware Requirements

| Item | Component Name | Description                                 | Quantity |  Notes              |
|:----:|:--------------:|:-------------------------------------------:|:--------:|:-------------------:|
|  1   | Raspberry Pi 5 | core control unit of the overall system     | 1 (pcs)  | 4GB RAM, 64-bit OS recommended |
|  2   | Pi Camera Module| Video and still image capture               | 1 (pcs)  | CSI-2 interface, 8MP  | 
|  3   | MAX30102       | ECG signals capture                         | 1 (pcs)  | I2C address 0x57, 3.3V logic level |
|  4   | Accessories    |  cables, power supply and expansion boards   | 1 (set)  | 5V/3A USB-C PD  | 

# Software Dependencies

## OS & Language

- **OS**: Linux (ARM-compatible).
- **Language**: C++.

## Libraries & Tools

- **Computer Vision**: OpenCV (video processing), dlib (face detection).
- **ML Inference**: ONNX Runtime (emotion classification model).
- **GUI**: Qt 6, Qt Charts (visualization).
- **Multimedia**: GStreamer.

# Key Code
## 1. Emotion Monitoring
### Face Detection & Scaling
```bash
# Downscale frame for faster detection
Mat small_frame;
resize(frame, small_frame, Size(), 0.5, 0.5);

# Detect faces every 5th frame
if (frame_count % DETECT_EVERY_N_FRAMES == 0) {
    dlib::cv_image<dlib::bgr_pixel> dlib_img(small_frame);
    auto faces = detector(dlib_img);
    
    # Convert coordinates to original frame size
    for (auto& f : faces) {
        Rect r(Point(f.left()*2, f.top()*2), 
              Point(f.right()*2, f.bottom()*2));
        cached_faces.push_back(r & Rect(0, 0, frame.cols, frame.rows));
    }
}
```

### Multi-threaded Inference
```bash
vector<thread> threads;
vector<int> max_idxs(faces.size(), 0);

    # Parallel processing for detected faces
    for (size_t i = 0; i < faces.size(); ++i) {
        threads.emplace_back([&, i]() {

            # Preprocess face ROI
            Mat face = frame(faces[i]).clone();
            resize(face, Size(224, 224));
            face.convertTo(face, CV_32F, 1.0/255.0);
   ```
* Convert BGR → CHW format
  ```bash
            vector<float> input_tensor;
            for (int c = 0; c < 3; ++c)
                for (int y = 0; y < 224; ++y)
                    for (int x = 0; x < 224; ++x)
                        input_tensor.push_back(face.at<Vec3f>(y, x)[c]);
  ```
* ONNX Runtime inference
  ```bash
            Ort::Value input_tensor_onnx = Ort::Value::CreateTensor<float>(...);
            auto outputs = session.Run(...);
  ```
* Softmax normalization
  ```bash
            float* logits = outputs[0].GetTensorMutableData<float>();
            vector<float> probs(7);
            transform(logits, logits+7, probs.begin(), [](float v){ return exp(v); });
            float sum = accumulate(probs.begin(), probs.end(), 0.0f);
            for (auto& p : probs) p /= sum;
            # Store prediction
            max_idxs[i] = max_element(probs.begin(), probs.end()) - probs.begin();
        });
    }
    for (auto &t : threads) t.join();
  ```

### Emotion Mapping & Statistics
```bash
# Map 7-class outputs to 3 categories
const array<int, 7> CATEGORY_MAPPING = {0, 0, 0, 1, 0, 1, 2}; 

# Track dominant emotion in first 10 seconds
vector<int> category_counts(3, 0);
auto start_time = chrono::steady_clock::now();

# Update statistics
double elapsed = chrono::duration_cast<chrono::seconds>(now - start_time).count();
if (elapsed <= 10.0) {
    category_counts[CATEGORY_MAPPING[max_idx]]++;
} else if (!reported) {
    cout << "Dominant emotion: " << CATEGORY_LABELS[argmax(category_counts)] << endl;
}
```

### Result Visualization
```bash
# Draw bounding boxes and labels
for (size_t i = 0; i < faces.size(); ++i) {
    int cat = CATEGORY_MAPPING[max_idxs[i]];
    putText(frame, CATEGORY_LABELS[cat], Point(faces[i].x, faces[i].y - 5),
            FONT_HERSHEY_SIMPLEX, 0.6, Scalar(0, 255, 0), 2);
    rectangle(frame, faces[i], Scalar(0, 255, 0), 2);
}

# Display output
imshow("MobileNet Emotion", frame);
```

## 2. HeartRate Monitoring
### Real-Time Data Acquisition Thread
```bash
class HeartWorker : public QObject {
    Q_OBJECT
    void startWorking() {
        while (true) {
            int32_t red, ir;
            max30102_read_fifo(i2cFile, &red, &ir);  # Read sensor data
            emit newIRSample(ir);  # Raw IR values for waveform
            
            if (bufferIndex >= 200) {  # Process every 200 samples
                low_pass_filter(irBuffer, 200);      # Noise reduction
                float bpm = calculate_heart_rate(irBuffer, 200);
                emit newBPMAvailable(bpm * 1.8f);    # Calibration factor
                bufferIndex = 0;
            }
            QThread::msleep(5);  
        }
    }
};
```

### Signal Processing Algorithms
* Low-Pass Filter
```bash
void low_pass_filter(int32_t* buffer, int length) {
    float alpha = 0.1f;  
    for (int i = 1; i < length; ++i)
        buffer[i] = alpha * buffer[i] + (1 - alpha) * buffer[i - 1];
}
```

* Heart Rate Calculation
```bash
float calculate_heart_rate(int32_t* buf, int len) {
    int peaks = 0, threshold = 1000;
    for (int i = 1; i < len - 1; ++i) {
        if (buf[i] > buf[i-1] && buf[i] > buf[i+1] && buf[i] > threshold)
            peaks++;
    }
    return (peaks / float(len)) * 60.0f * 2.0f;  # Convert to BPM
}
```

### Real-Time Visualization
```bash
# Update IR waveform display
void MainWindow::appendIRSample(int value) {
    static QVector<QPointF> buf;
    buf.append(QPointF(logicalX++, value));
    if (buf.size() > 600) buf.remove(0);  # Keep 3 seconds of data
    
    series->replace(buf);  # Update chart
    axisX->setRange(buf.first().x(), buf.last().x());
    
    # Auto-scale Y-axis
    int minY = value, maxY = value;
    for (auto& pt : buf) {
        minY = qMin(minY, int(pt.y()));
        maxY = qMax(maxY, int(pt.y()));
    }
    axisY->setRange(minY - 50, maxY + 50);  # Add 50-unit margin
}
```

### Heart Rate Statistics & Alerting
```bash
void MainWindow::updateBPM(float bpm) {
    # Update 10-second rolling window
    qint64 now = QDateTime::currentMSecsSinceEpoch();
    bpmHistory.append(qMakePair(now, bpm));
    
    # Remove old entries
    while (bpmHistory.first().first < now - 10000)
        bpmHistory.removeFirst();
    
    # Calculate max BPM
    float rollingMax = 0;
    for (auto& rec : bpmHistory)
        rollingMax = qMax(rollingMax, rec.second);
```
* Alert logic
```bash
    statusLabel->setText(bpm > 100 ? "Status: Elevated" : "Status: Normal");
    if (!tenSecondsProcessed && (now - startTime >= 10000)) {
        qDebug() << "10s Max BPM:" << rollingMax 
                 << (rollingMax <= 155 ? "Normal" : "Abnormal");
    }
}
```

# Installation Guide 
## 1. Hardware Setup  
- **Camera**:  
  1. Connect the camera to the CSI port.  
  2. Enable the interface:  
     ```bash  
     sudo raspi-config → Interface Options → Camera → Enable  
     ```  
- **MAX30102**:  
  1. Connect SDA and SCL pins.  
  2. Enable I2C:  
     ```bash  
     sudo raspi-config → Interface Options → I2C → Enable  
     sudo reboot  
     ```  
  3. Verify connection:  
     ```bash  
     sudo i2cdetect -y 1  
     ```  

## 2. Software Setup  
```bash  
# Install dependencies  
sudo apt update && sudo apt install -y \  
    libopencv-dev libdlib-dev libgstreamer1.0-dev \  
    qt6-base-dev qt6-charts-dev  

# Clone the repository  
git clone https://github.com/Ww1247/ECG-Assisted-Emotion-Monitoring.git  
cd ECG-Assisted-Emotion-Monitoring  

```

# FAQ
## Q1: Camera Not Detected
```bash
# Check CSI connection  
sudo vcgencmd get_camera  
# Expected output: supported=1 detected=1
```

## Q2: MAX30102 I2C Errors
1. Verify wiring (SDA=GPIO2, SCL=GPIO3).
2. Check sensor address:
```bash
sudo i2cdetect -y 1  
# Should show device at 0x57
```

# Version Information

Current Version: 1.0.0

Release Date: 20th April 2025

# Contact
* Email: 2935682G@student.gla.ac.uk

# Contributors

<!---
npx contributor-faces --exclude "*bot*" --limit 70 --repo "https://github.com/amplication/amplication"

change the height and width for each of the contributors from 80 to 50.
--->

<img src="https://avatars.githubusercontent.com/u/199613138?v=4" title="Student_Name" width="50" height="50" style="border-radius: 50%; object-fit: cover;"> <img src="https://avatars.githubusercontent.com/u/195966921?v=4" title="Student_Name" width="50" height="50" style="border-radius: 50%; object-fit: cover;"> <img src="https://avatars.githubusercontent.com/u/201265472?s=400&v=4" title="Student_Name" width="50" height="50" style="border-radius: 50%; object-fit: cover;"> <img src="https://avatars.githubusercontent.com/u/128835450?s=400&u=b5ecb292c1a067e392b97c62b5ac3e576df1d874&v=4" title="Student_Name" width="50" height="50" style="border-radius: 50%; object-fit: cover;">

| Name         | ID       | Role                                                                                                                              |
| ------------ |:--------:| --------------------------------------------------------------------------------------------------------------------------------- |
| Jiajun Wang  | 3007288W | Responsible for the QT part of the code, including GUI design and communication code creation.                                    |
| Xin Guo      | 2935682G | Responsible for Emotion Monitoring part of the code.                                                                              |
| Shaohan Tian | 3025822T | Responsible for hardware installation, HeartRate part of the code and video producing.                                                             |
| Xinyu Chen   | 2867562C | Responsible for HeartRate part of the code.                                                                                       |

# License

MIT License Copyright (c) 2025 Wx1247


[//]: contributor-faces

[//]: contributor-faces
