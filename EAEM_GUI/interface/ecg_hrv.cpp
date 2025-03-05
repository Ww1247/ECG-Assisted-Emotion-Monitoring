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
    QRadioButton *radioButton_ECG_display = new QRadioButton("ECG Display");
    QRadioButton *radioButton_blood_oxygen_display = new QRadioButton("Blood Oxygen");
    QRadioButton *radioButton_avg_heart_rate_display = new QRadioButton("Avg Heart Rate");
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

    QCustomPlot *customPlot_ECG = new QCustomPlot(frame_ECG);
    frame_ECG->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    layout_ECG->addWidget(customPlot_ECG);

    // Initialize ECG Plot
    setupPlot(customPlot_ECG, 0, 100, "");

    QVBoxLayout *verticalLayout_ECG = new QVBoxLayout;
    verticalLayout_ECG->addLayout(horizontalLayout_ECG_config);
    verticalLayout_ECG->addWidget(frame_ECG);

    groupBox_ECG->setLayout(verticalLayout_ECG);

    // HRV Group Box
    QGroupBox *groupBox_HRV = new QGroupBox("HRV", this);

    // HRV Display Options
    QRadioButton *radioButton_HRV_RMSSD = new QRadioButton("RMSSD");
    QRadioButton *radioButton_HRV_SDNN = new QRadioButton("SDNN");
    QRadioButton *radioButton_HRV_LF_HF = new QRadioButton("LF/HF");
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

    QCustomPlot *customPlot_HRV = new QCustomPlot(frame_HRV);
    frame_HRV->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    layout_HRV->addWidget(customPlot_HRV);

    // Initialize HRV Plot
    setupPlot(customPlot_HRV, 0, 100, "");

    QVBoxLayout *verticalLayout_HRV = new QVBoxLayout;
    verticalLayout_HRV->addLayout(horizontalLayout_HRV_config);
    verticalLayout_HRV->addWidget(frame_HRV);

    groupBox_HRV->setLayout(verticalLayout_HRV);

    // Main Layout
    QHBoxLayout *horizontalLayout_MAX30102 = new QHBoxLayout;
    horizontalLayout_MAX30102->addWidget(groupBox_ECG);
    horizontalLayout_MAX30102->addWidget(groupBox_HRV);

    setLayout(horizontalLayout_MAX30102);
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
