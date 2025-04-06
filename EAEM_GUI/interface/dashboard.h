#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <QWidget>
#include <QPushButton>
#include <QGroupBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QTime>
#include <QTimer>
#include <QLabel>
#include <QDateTime>

/**
 * @brief The DashboardWidget class provides a control panel for the emotion detection system.
 *
 * This widget contains start/stop buttons for triggering emotion detection,
 * and displays system time and date. It emits control signals to the rest of the application.
 */
class DashboardWidget : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief Constructs a DashboardWidget object.
     * @param parent The parent widget.
     */
    explicit DashboardWidget(QWidget *parent = nullptr);

    /**
     * @brief Enables the "Start Emotion Detection" button and disables the stop button.
     */
    void set_pushbuton_enable_start();

    /**
     * @brief Enables the "Stop Emotion Detection" button and disables the start button.
     */
    void set_pushbuton_enable_stop();

private:
    QPushButton *pushButton_start_emotion_detection; ///< Start button for emotion detection.
    QPushButton *pushButton_stop_emotion_detection;  ///< Stop button for emotion detection.

    /**
     * @brief Initializes the user interface elements.
     */
    void initUI();

    /**
     * @brief Updates the system date and time labels.
     * @param dateLabel Pointer to QLabel to display the current date.
     * @param timeLabel Pointer to QLabel to display the current time.
     */
    static void updateSystemDateTime(QLabel *dateLabel, QLabel *timeLabel);

private slots:
    /**
     * @brief Slot function called when the start button is clicked.
     */
    void on_pushButton_EmotionDetection_Start_Clicked();

    /**
     * @brief Slot function called when the stop button is clicked.
     */
    void on_pushButton_EmotionDetection_Stop_Clicked();

signals:
    /**
     * @brief Signal emitted when the user requests to start emotion detection.
     */
    void sig_emotion_detection_start();

    /**
     * @brief Signal emitted when the user requests to stop emotion detection.
     */
    void sig_emotion_detection_stop();
};

#endif // DASHBOARD_H
