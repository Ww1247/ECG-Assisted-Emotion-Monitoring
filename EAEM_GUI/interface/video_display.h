#ifndef VIDEODISPLAY_H
#define VIDEODISPLAY_H

#include <QWidget>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QComboBox>
#include <QImage>
#include <QPixmap>

#include "camera_driver.h"

class VideoDisplayWidget : public QWidget {

    Q_OBJECT

public:
    explicit VideoDisplayWidget(QWidget *parent = nullptr);
    ~VideoDisplayWidget();

    void initCamera();
    void start(int cameraIndex = 0);            // Start the camera
    void stop();                                // Stop the camera
    void setFPS(int fps);                       // Set FPS
    void setResolution(int width, int height);  // Set resolution

public slots:
    void updateFrame(const QImage &frame);
    void on_comboBox_resolution_currentIndexChanged(int index);
    void on_comboBox_video_fps_currentIndexChanged(int index);

private:
    QComboBox *comboBox_resolution;
    QComboBox *comboBox_video_fps;
    QLabel *label_video_display;
    CameraDriver *cameraDriver;
    QImage image;

    void initUI();

private slots:

signals:
    void frameReady(const QImage &frame);

};

#endif // VIDEODISPLAY_H
