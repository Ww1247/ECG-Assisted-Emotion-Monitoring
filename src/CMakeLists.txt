cmake_minimum_required(VERSION 3.16)
 
project(FACEqt)
 
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)  
 
find_package(OpenCV REQUIRED)
find_package(dlib REQUIRED)
find_package(Qt5 REQUIRED COMPONENTS Core Widgets Charts)
 
# 设置 ONNX Runtime 安装路径（根据实际情况修改）
set(ONNXRUNTIME_DIR "/home/pi/Desktop/onnxruntime-linux-aarch64-1.17.0")
include_directories(${ONNXRUNTIME_DIR}/include)
link_directories(${ONNXRUNTIME_DIR}/lib)
 
# ------------------ Executable ------------------
add_executable(${PROJECT_NAME}
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
)
 
# ------------------- Linking --------------------
target_link_libraries(${PROJECT_NAME}
    Qt5::Core
    Qt5::Widgets
    Qt5::Charts
    ${OpenCV_LIBS}
    dlib::dlib
    onnxruntime
)