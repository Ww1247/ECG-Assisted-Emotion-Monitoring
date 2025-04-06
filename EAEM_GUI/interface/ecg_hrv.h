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

/**
 * @brief The EcgHrvWidget class provides a visual interface for displaying ECG, SpO2, and HRV data.
 *
 * It includes multiple selectable plots and supports data updates from external sources in real time.
 * This widget inherits from PlotRefreshInterface and is registered with PlotRefreshManager for timed updates.
 */
class EcgHrvWidget : public QWidget, public PlotRefreshInterface
{
    Q_OBJECT

public:
    /**
     * @brief Constructs the ECG/HRV display widget.
     * @param parent The parent QWidget.
     */
    explicit EcgHrvWidget(QWidget *parent = nullptr);

    /**
     * @brief Updates the plot according to the latest selected data mode.
     */
    void updatePlot() override;

    /**
     * @brief Enables or disables the entire UI of the widget.
     * @param status If true, enables; if false, disables.
     */
    void set_UI_EnableDisable(const bool &status);

public slots:
    /**
     * @brief Adds ECG heart rate and SpO2 data to the buffer for plotting.
     * @param heartRate Measured heart rate value.
     * @param spo2 Measured blood oxygen level.
     */
    void addECGData(float heartRate, float spo2);

    /**
     * @brief Adds HRV data to the buffer.
     * @param rmssd Root Mean Square of Successive Differences.
     * @param sdnn Standard Deviation of NN intervals.
     * @param lf_hf Ratio of low-frequency to high-frequency components.
     */
    void addHRVData(float rmssd, float sdnn, float lf_hf);

private:
    // UI controls for selecting plot mode
    QRadioButton *radioButton_ECG_display;              ///< Display raw ECG data.
    QRadioButton *radioButton_blood_oxygen_display;     ///< Display SpO2 level.
    QRadioButton *radioButton_avg_heart_rate_display;   ///< Display average heart rate.
    QRadioButton *radioButton_HRV_RMSSD;                ///< Display HRV: RMSSD metric.
    QRadioButton *radioButton_HRV_SDNN;                 ///< Display HRV: SDNN metric.
    QRadioButton *radioButton_HRV_LF_HF;                ///< Display HRV: LF/HF ratio.

    // Plotting components
    QCustomPlot *customPlot_ECG;                        ///< Plot widget for ECG and SpO2.
    QCustomPlot *customPlot_HRV;                        ///< Plot widget for HRV metrics.

    // Data storage
    QElapsedTimer elapsed_;                             ///< Timer used to track real-time plotting.
    QVector<double> time_;                              ///< Time axis for data plotting.
    QVector<double> ecgData_;                           ///< Raw ECG data.
    QVector<double> spo2Data_;                          ///< SpO2 data.
    QVector<double> avgHRData_;                         ///< Average heart rate data.
    QVector<double> hrvRMSSD_;                          ///< HRV RMSSD metric.
    QVector<double> hrvSDNN_;                           ///< HRV SDNN metric.
    QVector<double> hrvLFHF_;                           ///< HRV LF/HF ratio.

    const int maxPoints_ = 300;                         ///< Maximum number of points to keep in plot buffers.

    /**
     * @brief Initializes the widget layout and control elements.
     */
    void initUI();

    /**
     * @brief Sets up a QCustomPlot with predefined axis and label settings.
     * @param plot Pointer to the plot to initialize.
     * @param yMin Minimum Y-axis value.
     * @param yMax Maximum Y-axis value.
     * @param yLabel Label text for the Y-axis.
     */
    void setupPlot(QCustomPlot *plot, double yMin, double yMax, const QString &yLabel);

    /**
     * @brief Redraws the plots based on current data and selected mode.
     */
    void updatePlots();
};


#endif // ECG_HRV_H
