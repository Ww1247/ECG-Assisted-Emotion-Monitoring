#ifndef ECG_HRV_H
#define ECG_HRV_H

#include <QWidget>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QRadioButton>
#include <QFrame>
#include <QSpacerItem>
#include "qcustomplot.h"


class EcgHrvWidget : public QWidget {
    Q_OBJECT
public:
    explicit EcgHrvWidget(QWidget *parent = nullptr);

    void set_UI_EnableDisable(const bool &status);

public slots:


private:
    void initUI();
    void setupPlot(QCustomPlot *plot, double yMin, double yMax, const QString &yLabel);

    QRadioButton *radioButton_ECG_display;
    QRadioButton *radioButton_blood_oxygen_display;
    QRadioButton *radioButton_avg_heart_rate_display;

    QRadioButton *radioButton_HRV_RMSSD;
    QRadioButton *radioButton_HRV_SDNN;
    QRadioButton *radioButton_HRV_LF_HF;
};

#endif // ECG_HRV_H
