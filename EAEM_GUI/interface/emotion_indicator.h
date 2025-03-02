#ifndef EMOTIONINDICATOR_H
#define EMOTIONINDICATOR_H

#include <QWidget>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFrame>
#include <QTextEdit>

class EmotionIndicatorWidget : public QWidget {
    Q_OBJECT
public:
    explicit EmotionIndicatorWidget(QWidget *parent = nullptr);

private:
    void initUI();

    QGroupBox *groupBox;
    QFrame *frameJoyful;
    QFrame *frameRelaxed;
    QFrame *frameAnxious;
    QLabel *labelJoyful;
    QLabel *labelRelaxed;
    QLabel *labelAnxious;
    QTextEdit *textEditInfo;
};

#endif // EMOTIONINDICATOR_H
