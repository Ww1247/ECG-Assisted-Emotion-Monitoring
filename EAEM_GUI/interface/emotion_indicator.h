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

class EmotionIndicatorWidget : public QWidget {
    Q_OBJECT

public:
    explicit EmotionIndicatorWidget(QWidget *parent = nullptr);

    void replace_textEditInfo_Display(const QString &message);
    void append_textEditInfo_Display(const QString &message);
    void emotion_status_receiver(const QString &emotion, const QString &confident);

private:
    QGroupBox *groupBox;
    QFrame *frameJoyful;
    QFrame *frameRelaxed;
    QFrame *frameAnxious;
    QLabel *labelJoyful;
    QLabel *labelRelaxed;
    QLabel *labelAnxious;
    QTextEdit *textEditInfo;

    void initUI();
};

#endif // EMOTIONINDICATOR_H
