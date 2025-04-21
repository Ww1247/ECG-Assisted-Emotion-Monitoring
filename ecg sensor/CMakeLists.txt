
cmake_minimum_required(VERSION 3.10)
project(HeartRateWithPlot)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 查找 Qt5 所需模块
find_package(Qt5 REQUIRED COMPONENTS Core Widgets Charts)

# 包含当前目录头文件
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# 添加可执行文件
add_executable(HeartRateWithPlot
    main_threaded_with_plot.cpp
    mainwindow_threaded_with_plot.cpp
    mainwindow_threaded_with_plot.h
)

# 链接 Qt5 库
target_link_libraries(HeartRateWithPlot
    Qt5::Core
    Qt5::Widgets
    Qt5::Charts
)
