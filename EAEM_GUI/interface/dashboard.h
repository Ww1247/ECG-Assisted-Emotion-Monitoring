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

private:
    void initUI();
    static void updateSystemDateTime(QLabel *dateLabel, QLabel *timeLabel);
};

#endif // DASHBOARD_H
