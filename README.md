<div align="center">
    <img src="Documents/pictures/EAEM-Logo.png"/ width=300>
    <p>
      <i align="center">An ECG-Assisted Emotion Monitoring System that captures facial expressions via a camera and extracts micro-expression features using image recognition. By integrating ECG signals, the system aims to assess physical exertion and emotional strain during exercise.🫀</i>
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
       <a href="facebook_url"><img src="https://img.shields.io/badge/Facebook-%231877F2.svg?logo=Facebook&logoColor=white"/></a>
       <a href="instagram_url"><img src="https://img.shields.io/badge/Instagram-%23E4405F.svg?logo=Instagram&logoColor=white"/></a>
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

# Build the project  
mkdir build && cd build  
cmake -DCMAKE_BUILD_TYPE=Release ..  
make -j4  

# Run the application  
./EmotionMonitoringApp  
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
| Shaohan Tian | 3025822T | Responsible for hardware installation and HeartRate part of the code.                                                             |
| Xinyu Chen   | 2867562C | Responsible for HeartRate part of the code.                                                                                       |

# License

MIT License Copyright (c) 2025 Wx1247


[//]: contributor-faces

[//]: contributor-faces
