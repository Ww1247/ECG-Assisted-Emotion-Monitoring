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

class DashboardWidget : public QWidget {
    Q_OBJECT
public:
    explicit DashboardWidget(QWidget *parent = nullptr);
    void pushButton_start_emotion_detection_clicked();
    void pushButton_stop_emotion_detection_clicked();

private:
    QPushButton *pushButton_start_emotion_detection;
    QPushButton *pushButton_stop_emotion_detection;

    void initUI();
    static void updateSystemDateTime(QLabel *dateLabel, QLabel *timeLabel);

signals:
    void sig_emotion_detection_start();
    void sig_emotion_detection_stop();
};

#endif // DASHBOARD_H
