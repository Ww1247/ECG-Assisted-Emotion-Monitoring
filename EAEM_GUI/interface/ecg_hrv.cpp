#include "ecg_hrv.h"

EcgHrvWidget::EcgHrvWidget(QWidget *parent)
    : QWidget(parent)
{
    initUI();
}

void EcgHrvWidget::initUI()
{
    // ECG Group Box
    QGroupBox *groupBox_ECG = new QGroupBox("ECG", this);

    // ECG Display Options
    radioButton_ECG_display = new QRadioButton("ECG Display", this);
    radioButton_blood_oxygen_display = new QRadioButton("Blood Oxygen", this);
    radioButton_avg_heart_rate_display = new QRadioButton("Avg Heart Rate", this);
    radioButton_ECG_display->setChecked(true);

    QHBoxLayout *horizontalLayout_ECG_config = new QHBoxLayout;
    horizontalLayout_ECG_config->addWidget(radioButton_ECG_display);
    horizontalLayout_ECG_config->addWidget(radioButton_blood_oxygen_display);
    horizontalLayout_ECG_config->addWidget(radioButton_avg_heart_rate_display);
    horizontalLayout_ECG_config->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

    // ECG Display Frame with QCustomPlot
    QFrame *frame_ECG = new QFrame;
    frame_ECG->setMinimumSize(450, 150);
    frame_ECG->setFrameShape(QFrame::Box);

    QVBoxLayout *layout_ECG = new QVBoxLayout(frame_ECG);
    layout_ECG->setContentsMargins(0, 0, 0, 0); // Remove margins
    layout_ECG->setSpacing(0); // Remove spacing

    customPlot_ECG = new QCustomPlot(frame_ECG);
    frame_ECG->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    layout_ECG->addWidget(customPlot_ECG);

    // Initialize ECG Plot
    setupPlot(customPlot_ECG, 0, 100, "");

    QVBoxLayout *verticalLayout_ECG = new QVBoxLayout;
    verticalLayout_ECG->addLayout(horizontalLayout_ECG_config);
    verticalLayout_ECG->addWidget(frame_ECG);

    groupBox_ECG->setLayout(verticalLayout_ECG);

#if CONFIG_HRV_UI_DISPLAY_ENABLE
    // HRV Group Box
    QGroupBox *groupBox_HRV = new QGroupBox("HRV", this);

    // HRV Display Options
    radioButton_HRV_RMSSD = new QRadioButton("RMSSD", this);
    radioButton_HRV_SDNN = new QRadioButton("SDNN", this);
    radioButton_HRV_LF_HF = new QRadioButton("LF/HF", this);
    radioButton_HRV_RMSSD->setChecked(true);

    QHBoxLayout *horizontalLayout_HRV_config = new QHBoxLayout;
    horizontalLayout_HRV_config->addWidget(radioButton_HRV_RMSSD);
    horizontalLayout_HRV_config->addWidget(radioButton_HRV_SDNN);
    horizontalLayout_HRV_config->addWidget(radioButton_HRV_LF_HF);
    horizontalLayout_HRV_config->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

    // HRV Display Frame with QCustomPlot
    QFrame *frame_HRV = new QFrame;
    frame_HRV->setMinimumSize(450, 150);
    frame_HRV->setFrameShape(QFrame::Box);

    QVBoxLayout *layout_HRV = new QVBoxLayout(frame_HRV);
    layout_HRV->setContentsMargins(0, 0, 0, 0); // Remove margins
    layout_HRV->setSpacing(0); // Remove spacing

    customPlot_HRV = new QCustomPlot(frame_HRV);
    frame_HRV->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    layout_HRV->addWidget(customPlot_HRV);

    // Initialize HRV Plot
    setupPlot(customPlot_HRV, 0, 100, "");

    QVBoxLayout *verticalLayout_HRV = new QVBoxLayout;
    verticalLayout_HRV->addLayout(horizontalLayout_HRV_config);
    verticalLayout_HRV->addWidget(frame_HRV);

    groupBox_HRV->setLayout(verticalLayout_HRV);
#endif

    // Main Layout
    QHBoxLayout *horizontalLayout_MAX30102 = new QHBoxLayout;
    horizontalLayout_MAX30102->addWidget(groupBox_ECG);

#if CONFIG_HRV_UI_DISPLAY_ENABLE
    horizontalLayout_MAX30102->addWidget(groupBox_HRV);
#endif

    setLayout(horizontalLayout_MAX30102);

    elapsed_.start();
}

// Function to initialize QCustomPlot settings
void EcgHrvWidget::setupPlot(QCustomPlot *plot, double yMin, double yMax, const QString &yLabel) {
    // Add a graph to the customPlot and store it in the graph pointer
    plot->addGraph(); // Ensure graph is a QCPGraph*
    plot->graph(0)->setPen(QPen(Qt::green, 2)); // Set line color and thickness

    // Configure X axis (time in milliseconds)
    plot->xAxis->setTickLabels(false);
    plot->xAxis->setBasePen(QPen(Qt::white));
    plot->xAxis->setTickPen(QPen(Qt::white));
    plot->xAxis->setSubTickPen(QPen(Qt::white));
    plot->xAxis->setTickLabelColor(Qt::white); // Set X-axis tick color
    plot->xAxis->setLabel("Time (ms)"); // Set X-axis label
    plot->xAxis->setLabelColor(Qt::white); // Set X-axis label color

    // Configure Y axis
    plot->yAxis->setRange(yMin, yMax);
    plot->yAxis->setTickLabels(true);
    plot->yAxis->setNumberPrecision(2);
    plot->yAxis->ticker()->setTickCount(5); // Control tick count
    plot->yAxis->setLabel(yLabel); // Set Y-axis label
    plot->yAxis->setLabelColor(Qt::white); // Set Y-axis label color
    plot->yAxis->setBasePen(QPen(Qt::white));
    plot->yAxis->setTickPen(QPen(Qt::white));
    plot->yAxis->setSubTickPen(QPen(Qt::white));
    plot->yAxis->setTickLabelColor(Qt::white); // Set tick label color

    // Set background color
    plot->setBackground(QBrush(Qt::black));

    // Disable grid lines
    plot->xAxis->grid()->setVisible(false);
    plot->yAxis->grid()->setVisible(false);

    // Enable user interactions for zooming and panning
    plot->setInteraction(QCP::iRangeDrag, true);
    plot->setInteraction(QCP::iRangeZoom, true);

    // Prevent the view from resetting during data updates
    // customPlot->axisRect()->setRangeZoom(Qt::Horizontal | Qt::Vertical);
    // customPlot->axisRect()->setRangeDrag(Qt::Horizontal | Qt::Vertical);
    plot->axisRect()->setRangeZoom(Qt::Horizontal);
    plot->axisRect()->setRangeDrag(Qt::Horizontal);
}

void EcgHrvWidget::set_UI_EnableDisable(const bool &status)
{
    radioButton_ECG_display->setEnabled(status);
    radioButton_blood_oxygen_display->setEnabled(status);
    radioButton_avg_heart_rate_display->setEnabled(status);

#if CONFIG_HRV_UI_DISPLAY_ENABLE
    radioButton_HRV_RMSSD->setEnabled(status);
    radioButton_HRV_SDNN->setEnabled(status);
    radioButton_HRV_LF_HF->setEnabled(status);
#endif
}

void EcgHrvWidget::addECGData(float heartRate, float spo2)
{
    double t = elapsed_.elapsed();
    time_ << t;
    ecgData_ << heartRate;
    spo2Data_ << spo2;
    avgHRData_ << (ecgData_.size() > 0 ? std::accumulate(ecgData_.begin(), ecgData_.end(), 0.0) / ecgData_.size() : heartRate);

    // Refresh data
    while (time_.size() > maxPoints_) {
        time_.removeFirst();
        ecgData_.removeFirst();
        spo2Data_.removeFirst();
        avgHRData_.removeFirst();
    }
}

#if CONFIG_HRV_UI_DISPLAY_ENABLE
void EcgHrvWidget::addHRVData(float rmssd, float sdnn, float lf_hf)
{
    hrvRMSSD_ << rmssd;
    hrvSDNN_ << sdnn;
    hrvLFHF_ << lf_hf;

    while (hrvRMSSD_.size() > maxPoints_) {
        hrvRMSSD_.removeFirst();
        hrvSDNN_.removeFirst();
        hrvLFHF_.removeFirst();
    }
}
#endif

void EcgHrvWidget::updatePlots()
{
    if (time_.isEmpty()) return;
    double now = time_.last();

    // ECG Curve
    if (radioButton_ECG_display->isChecked())
        customPlot_ECG->graph(0)->setData(time_, ecgData_);
    else if (radioButton_blood_oxygen_display->isChecked())
        customPlot_ECG->graph(0)->setData(time_, spo2Data_);
    else
        customPlot_ECG->graph(0)->setData(time_, avgHRData_);

    customPlot_ECG->xAxis->setRange(now - 5000, now);
    customPlot_ECG->yAxis->rescale();
    customPlot_ECG->replot(QCustomPlot::rpQueuedReplot);

#if CONFIG_HRV_UI_DISPLAY_ENABLE
    // HRV Curve
    QVector<double> x(hrvRMSSD_.size());
    for (int i = 0; i < x.size(); ++i)
        x[i] = i;

    if (radioButton_HRV_RMSSD->isChecked())
        customPlot_HRV->graph(0)->setData(x, hrvRMSSD_);
    else if (radioButton_HRV_SDNN->isChecked())
        customPlot_HRV->graph(0)->setData(x, hrvSDNN_);
    else
        customPlot_HRV->graph(0)->setData(x, hrvLFHF_);

    customPlot_HRV->xAxis->setRange(0, hrvRMSSD_.size());
    customPlot_HRV->yAxis->rescale();
    customPlot_HRV->replot(QCustomPlot::rpQueuedReplot);
#endif
}

void EcgHrvWidget::updatePlot()
{
    updatePlots();  // Timed refresh function
}

