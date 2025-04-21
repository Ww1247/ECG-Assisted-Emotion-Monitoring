#ifndef MAINWINDOW_THREADED_WITH_PLOT_H
#define MAINWINDOW_THREADED_WITH_PLOT_H

#include <QMainWindow>
#include <QLabel>
#include <QList>
#include <QPair>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QDateTime>
#include <QtCharts/QChartView>
#include <QPainter>


QT_CHARTS_USE_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

private slots:
    void updateBPM(float bpm);
    void appendIRSample(int value);

private:
    QLabel* rateLabel;                 // 实时心率
    QLabel* statusLabel;               // 实时状态
    QLabel* maxRateLabel;              // 滚动窗口内实时最大值 & 10s 最终输出

    QList<QPair<qint64, float>> bpmHistory;
    qint64 startTime;                  // 程序启动时刻
    bool tenSecondsProcessed;          // 标记“10s 后输出”是否已执行

    QChart* chart;
    QLineSeries* series;
    QValueAxis* axisX;
    QValueAxis* axisY;

    int i2cFile;
    int xValue;

    void initSensor();
    int i2c_write(int file, uint8_t reg, uint8_t value);
};

#endif // MAINWINDOW_THREADED_WITH_PLOT_H
