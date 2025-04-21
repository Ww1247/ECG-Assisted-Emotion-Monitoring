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

/**
 * @brief The VideoDisplayWidget class provides a GUI component for displaying live video frames
 * and configuring camera settings such as resolution and frame rate.
 */
class VideoDisplayWidget : public QWidget {

    Q_OBJECT

public:
    /**
     * @brief Constructs the VideoDisplayWidget.
     * @param parent The parent QWidget.
     */
    explicit VideoDisplayWidget(QWidget *parent = nullptr);

    /**
     * @brief Destructor for VideoDisplayWidget.
     */
    ~VideoDisplayWidget();

    /**
     * @brief Sets the camera frame rate (FPS).
     * @param fps Desired frames per second.
     */
    void setFPS(int fps);

    /**
     * @brief Sets the camera resolution.
     * @param width Desired width in pixels.
     * @param height Desired height in pixels.
     */
    void setResolution(int width, int height);

public slots:
    /**
     * @brief Slot triggered when the resolution selection is changed from the combo box.
     * @param index The selected index.
     */
    void on_comboBox_resolution_currentIndexChanged(int index);

    /**
     * @brief Slot triggered when the FPS selection is changed from the combo box.
     * @param index The selected index.
     */
    void on_comboBox_video_fps_currentIndexChanged(int index);

    /**
     * @brief Updates the video display label with a new frame.
     * @param frame The captured video frame as a QImage.
     */
    void updateFrame(const QImage &frame);

private:
    QComboBox *comboBox_resolution;     ///< Dropdown menu for selecting resolution.
    QComboBox *comboBox_video_fps;      ///< Dropdown menu for selecting frame rate.
    QLabel *label_video_display;        ///< QLabel used to display the live video frames.

    /**
     * @brief Initializes the UI layout and widgets.
     */
    void initUI();
};

#endif // VIDEODISPLAY_H
