/**
 * @file emotion_indicator.h
 * @brief Declaration of EmotionIndicatorWidget used for emotion display and feedback.
 */
#ifndef EMOTIONINDICATOR_H
#define EMOTIONINDICATOR_H

#include <QWidget>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFrame>
#include <QTextEdit>
#include "sensor_data.h"

/**
 * @brief The EmotionIndicatorWidget class visually presents real-time emotional state information.
 *
 * It provides graphical and textual display of recognized emotions such as Joyful, Relaxed, or Anxious.
 * It can receive updates and reflect changes dynamically in the UI.
 */
class EmotionIndicatorWidget : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief Constructs the emotion indicator widget.
     * @param parent The parent QWidget.
     */
    explicit EmotionIndicatorWidget(QWidget *parent = nullptr);

    /**
     * @brief Replaces the contents of the info text panel.
     * @param message The message string to set.
     */
    void replace_textEditInfo_Display(const QString &message);

    /**
     * @brief Appends a message to the info text panel.
     * @param message The message string to append.
     */
    void append_textEditInfo_Display(const QString &message);

    /**
     * @brief Receives the current emotion state and confidence level to update the display.
     * @param emotion The recognized emotion label.
     * @param confident The confidence score in string format.
     */
    void emotion_status_receiver(const QString &emotion, const QString &confident);

private:
    QGroupBox *groupBox;             ///< Container group box for layout.
    QFrame *frameJoyful;             ///< Visual indicator for Joyful emotion.
    QFrame *frameRelaxed;            ///< Visual indicator for Relaxed emotion.
    QFrame *frameAnxious;            ///< Visual indicator for Anxious emotion.
    QLabel *labelJoyful;             ///< Label for Joyful status.
    QLabel *labelRelaxed;            ///< Label for Relaxed status.
    QLabel *labelAnxious;            ///< Label for Anxious status.
    QTextEdit *textEditInfo;         ///< Text box for showing emotion detection logs or feedback.

    /**
     * @brief Initializes the user interface layout and elements.
     */
    void initUI();
};
#endif // EMOTIONINDICATOR_H
