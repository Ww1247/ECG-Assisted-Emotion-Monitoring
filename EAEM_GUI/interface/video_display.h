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
    QComboBox *comboBox_resolution;
    QComboBox *comboBox_video_fps;

    void initUI();

private slots:
    void on_comboBox_resolution_currentIndexChanged(int index);
    void on_comboBox_video_fps_currentIndexChanged(int index);

};

#endif // VIDEODISPLAY_H
