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
  - Uses **dlib** for face detection and **ONNX Runtime** to classify emotions (positive/negative/neutral).  
  - Displays emotion probabilities (softmax-normalized scores) on video frames with bounding boxes and labels.

## 2. Heart Rate Monitoring 
- **Hardware**: MAX30102 sensor (I2C interface).  
- **Workflow**:  
  - Collects IR data for heart rate calculation.  
  - Applies low-pass filtering and peak detection to derive heart rate (BPM).  
  - Visualizes real-time PPG waveforms and BPM values using **Qt Charts** in a GUI.  

# Hardware Requirements 
| Item | Component Name | Part Number | Description                                 | Quantity |  Notes              | Datasheet |
|:----:|:--------------:|:-----------:|:-------------------------------------------:|:--------:|:-------------------:|:---------:|
|  1   | Raspberry Pi 5 |             | core control unit of the overall system     | 1 (pcs)  |                     |           |
|  2   | Camera         |             | Video and still image capture               | 1 (pcs)  | Facial recognition  |           |
|  3   | MAX30102       |             | ECG signals capture                         | 1 (pcs)  |                     |           |
|  4   | Accessories    |             | cables, power supply and expansion boards   |          |                     |           |

# Software Dependencies  

## OS & Language  
- **OS**: Linux (ARM-compatible).  
- **Language**: C++.  

## Libraries & Tools  
- **Computer Vision**: OpenCV (video processing), dlib (face detection).  
- **ML Inference**: ONNX Runtime (emotion classification model).  
- **GUI**: Qt 6, Qt Charts (visualization).  
- **Multimedia**: GStreamer.

# Hardware Setup  

## Camera Configuration  
1. Connect the Raspberry Pi Camera to the CSI port.  
2. Enable the camera interface.  

## MAX30102 Sensor Setup  
1. Connect the sensor to the Raspberry Pi via I2C pins.  
2. Enable I2C in raspi-config and verify connectivity using i2cdetect.

# Version Information
Current Version: 1.0.0

Release Date: April 2025

# Contributors

<!---
npx contributor-faces --exclude "*bot*" --limit 70 --repo "https://github.com/amplication/amplication"

change the height and width for each of the contributors from 80 to 50.
--->

[//]: contributor-faces

  <img src="https://avatars.githubusercontent.com/u/199613138?v=4" title="Student_Name" width="50" height="50" style="border-radius: 50%; object-fit: cover;"> <img src="https://avatars.githubusercontent.com/u/128835450?s=400&u=b5ecb292c1a067e392b97c62b5ac3e576df1d874&v=4" title="Student_Name" width="50" height="50" style="border-radius: 50%; object-fit: cover;">

[//]: contributor-faces

| Name         | ID       | Role                                                                                                                              |
| ------------ |:--------:| --------------------------------------------------------------------------------------------------------------------------------- |
|              | 3007288W | Responsible for the QT part of the code, including GUI design and communication code creation.                                    |
|              |          |                                                                                                                                   |
|              |          |                                                                                                                                   |
| Xinyu Chen   | 2867562C | Responsible for HeartRate part of the code.                                                                                       |
|              |          |                                                                                                                                   |

# License
MIT License Copyright (c) 2025 Wx1247
