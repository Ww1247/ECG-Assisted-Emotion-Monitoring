#ifndef VIDEODISPLAY_H
#define VIDEODISPLAY_H

#include <QWidget>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QComboBox>

class VideoDisplayWidget : public QWidget {
    Q_OBJECT
public:
    explicit VideoDisplayWidget(QWidget *parent = nullptr);

private:
    void initUI();
};

#endif // VIDEODISPLAY_H
