cmake_minimum_required(VERSION 3.5)

project(EAEM_GUI VERSION 0.1 LANGUAGES CXX)

# Set C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

# Find Qt5 or Qt6
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets PrintSupport)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets PrintSupport)

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
        interface/plot_driver.h interface/plot_driver.cpp
        interface/qcustomplot.h interface/qcustomplot.cpp
        interface/mainwindow.h interface/mainwindow.cpp
    )
endif()

# Add directories to the include path.
target_include_directories(EAEM_GUI PRIVATE
        ${CMAKE_SOURCE_DIR}/interface
)

# Link Qt Widgets library
target_link_libraries(EAEM_GUI PRIVATE
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::PrintSupport)

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
