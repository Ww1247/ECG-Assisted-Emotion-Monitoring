cmake_minimum_required(VERSION 3.5)

project(EAEM_GUI VERSION 0.1 LANGUAGES CXX)

# Set C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

# Find Package
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets PrintSupport)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets PrintSupport)
find_package(OpenCV REQUIRED)

# Find Library
find_library(PIGPIO_LIBS NAMES pigpio REQUIRED)

# Project source files
set(PROJECT_SOURCES
        main.cpp
)

# If Qt version is 6, use qt_add_executable, otherwise use add_executable for Qt 5
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(EAEM_GUI
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    add_executable(EAEM_GUI
        ${PROJECT_SOURCES}
        driver/qcustomplot.h driver/qcustomplot.cpp
        driver/i2c_driver.h driver/i2c_driver.cpp
        driver/camera_driver.h driver/camera_driver.cpp

        interface/mainwindow.h interface/mainwindow.cpp
        interface/dashboard.h interface/dashboard.cpp
        interface/video_display.h interface/video_display.cpp
        interface/emotion_indicator.h interface/emotion_indicator.cpp
        interface/sensor_max30102.h interface/sensor_max30102.cpp
        interface/sensor_aht20.h interface/sensor_aht20.cpp
        interface/ecg_hrv.h interface/ecg_hrv.cpp
        interface/temperature_humidity.h interface/temperature_humidity.cpp

        ploting/plot_refresh_interface.h
        ploting/plot_refresh_manager.h ploting/plot_refresh_manager.cpp

        sensors/aht20.h sensors/aht20.cpp
        sensors/max30102.h sensors/max30102.cpp
        sensors/sensor_interface.h
        sensors/sensor_data.h
        sensors/sensor_task.h sensors/sensor_task.cpp
        sensors/sensor_manager.h sensors/sensor_manager.cpp
    )
endif()

# Add directories to the include path.
target_include_directories(EAEM_GUI PRIVATE
        ${CMAKE_SOURCE_DIR}/arithmetic
        ${CMAKE_SOURCE_DIR}/driver
        ${CMAKE_SOURCE_DIR}/interface
        ${CMAKE_SOURCE_DIR}/sensors
        ${CMAKE_SOURCE_DIR}/ploting
)

# Link Qt Widgets library
target_link_libraries(EAEM_GUI PRIVATE
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::PrintSupport
        ${OpenCV_LIBS}
        ${PIGPIO_LIBS})

# Install settings
include(GNUInstallDirs)
install(TARGETS EAEM_GUI
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Finalize Qt executable for Qt 6
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(EAEM_GUI)
endif()

configure_file(${CMAKE_SOURCE_DIR}/model/emotion-ferplus-2.onnx
               ${CMAKE_CURRENT_BINARY_DIR}/emotion-ferplus-2.onnx COPYONLY)

configure_file(${CMAKE_SOURCE_DIR}/model/haarcascade_frontalface_default.xml
               ${CMAKE_CURRENT_BINARY_DIR}/haarcascade_frontalface_default.xml COPYONLY)
