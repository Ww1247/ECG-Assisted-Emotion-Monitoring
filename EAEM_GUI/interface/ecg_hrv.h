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
#include "plot_refresh_manager.h"


class EcgHrvWidget : public QWidget, public PlotRefreshInterface
{
    Q_OBJECT

public:
    explicit EcgHrvWidget(QWidget *parent = nullptr);

    void updatePlot() override;
    void set_UI_EnableDisable(const bool &status);

public slots:
    void addECGData(float heartRate, float spo2);
    void addHRVData(float rmssd, float sdnn, float lf_hf);


private:
    QRadioButton *radioButton_ECG_display;
    QRadioButton *radioButton_blood_oxygen_display;
    QRadioButton *radioButton_avg_heart_rate_display;
    QRadioButton *radioButton_HRV_RMSSD;
    QRadioButton *radioButton_HRV_SDNN;
    QRadioButton *radioButton_HRV_LF_HF;
    QCustomPlot *customPlot_ECG;
    QCustomPlot *customPlot_HRV;
    QElapsedTimer elapsed_;
    QVector<double> time_;
    QVector<double> ecgData_;
    QVector<double> spo2Data_;
    QVector<double> avgHRData_;
    QVector<double> hrvRMSSD_;
    QVector<double> hrvSDNN_;
    QVector<double> hrvLFHF_;
    const int maxPoints_ = 300;

    void initUI();
    void setupPlot(QCustomPlot *plot, double yMin, double yMax, const QString &yLabel);
    void updatePlots();
};

#endif // ECG_HRV_H
