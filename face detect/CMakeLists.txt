
cmake_minimum_required(VERSION 3.10)
project(FaceEmotionSimple)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)
#include_directories(${OpenCV_INCLUDE_DIRS})

find_package(dlib REQUIRED)
#include_directories(${dlib_INCLUDE_DIRS})

# 设置 ONNX Runtime 安装路径（请根据实际路径修改）
set(ONNXRUNTIME_DIR "/home/pi/Desktop/onnxruntime-linux-aarch64-1.17.0")
 
# 引入 ONNX Runtime 头文件和库
include_directories(${ONNXRUNTIME_DIR}/include)
link_directories(${ONNXRUNTIME_DIR}/lib)

add_executable(face_emotion_simple main03.cpp)
target_link_libraries(face_emotion_simple onnxruntime ${OpenCV_LIBS} dlib::dlib)
