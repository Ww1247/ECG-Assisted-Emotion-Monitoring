#include "emotion_indicator.h"
#include <QDebug>

EmotionIndicatorWidget::EmotionIndicatorWidget(QWidget *parent)
    : QWidget(parent)
{
    initUI();
}

void EmotionIndicatorWidget::initUI()
{
    // Create main group box
    groupBox = new QGroupBox("Emotion Indicator", this);

    // Create emotion frames
    frameJoyful = new QFrame(this);
    frameRelaxed = new QFrame(this);
    frameAnxious = new QFrame(this);

    // Set frame properties
    QList<QFrame*> frames = {frameJoyful, frameRelaxed, frameAnxious};
    for (QFrame *frame : frames) {
        frame->setMinimumSize(75, 70);
        frame->setMaximumSize(75, 70);
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
    }

    // Create labels
    labelJoyful = new QLabel("Neutral", this);
    labelRelaxed = new QLabel("Happy", this);
    labelAnxious = new QLabel("Sad", this);

    // Align text center
    labelJoyful->setAlignment(Qt::AlignCenter);
    labelRelaxed->setAlignment(Qt::AlignCenter);
    labelAnxious->setAlignment(Qt::AlignCenter);

    // Set layouts for frames
    QGridLayout *gridLayoutJoyful = new QGridLayout(frameJoyful);
    QGridLayout *gridLayoutRelaxed = new QGridLayout(frameRelaxed);
    QGridLayout *gridLayoutAnxious = new QGridLayout(frameAnxious);

    gridLayoutJoyful->addWidget(labelJoyful, 0, 0);
    gridLayoutRelaxed->addWidget(labelRelaxed, 0, 0);
    gridLayoutAnxious->addWidget(labelAnxious, 0, 0);

    // Text display area
    textEditInfo = new QTextEdit(this);
    textEditInfo->setReadOnly(true);
    textEditInfo->setPlaceholderText("Emotion analysis results...");

    // Horizontal layout for emotions
    QHBoxLayout *emotionLayout = new QHBoxLayout;
    emotionLayout->addWidget(frameJoyful);
    emotionLayout->addWidget(frameRelaxed);
    emotionLayout->addWidget(frameAnxious);
    emotionLayout->addWidget(textEditInfo, 1);  // Expand text box

    // Main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(groupBox);
    mainLayout->addLayout(emotionLayout);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(groupBox);
}

void EmotionIndicatorWidget::replace_textEditInfo_Display(const QString &message)
{
    this->textEditInfo->setPlainText(message);
}

void EmotionIndicatorWidget::append_textEditInfo_Display(const QString &message)
{
    this->textEditInfo->append(message);
}

void EmotionIndicatorWidget::emotion_status_receiver(const QString &emotion, const QString &confident)
{
    this->append_textEditInfo_Display("Emotions:" + emotion + ", Confidents: " + confident);
    if (emotion == "Neutral"){
        labelJoyful->setStyleSheet("background-color: green;");
    }
    else if (emotion == "Happy"){
        labelRelaxed->setStyleSheet("background-color: green;");
    }
    else if (emotion == "Sad"){
        labelAnxious->setStyleSheet("background-color: green;");
    }
    else{
        labelJoyful->setStyleSheet("");
        labelRelaxed->setStyleSheet("");
        labelAnxious->setStyleSheet("");
    }
}


