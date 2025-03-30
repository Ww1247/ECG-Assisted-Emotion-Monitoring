#ifndef SENSOR_MAX30102_H
#define SENSOR_MAX30102_H

#include <QWidget>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QSlider>
#include <QSpacerItem>
#include <QComboBox>

class SensorMAX30102Widget : public QWidget {

    Q_OBJECT

public:
    explicit SensorMAX30102Widget(QWidget *parent = nullptr);
    ~SensorMAX30102Widget();

private:
    void initUI();

    // Combo boxes for sensor configuration
    QComboBox *comboBox_sampling_rate;
    QComboBox *comboBox_led_red;
    QComboBox *comboBox_led_ir;

    // Checkboxes for mode selection
    QCheckBox *checkBox_heart_rate_mode;
    QCheckBox *checkBox_blood_oxygen_mode;
    QCheckBox *checkBox_mixed_mode;

    // Sliders for real-time value adjustment
    QSlider *slider_heart_rate;
    QSlider *slider_blood_oxygen;

    QLineEdit *lineEdit_heart_rate_value;
    QLineEdit *lineEdit_blood_oxygen_value;

public slots:
    void updateValues(const float &heartrate, const float &spo2);

private slots:
    void on_comboBox_sampling_rate_currentIndexChanged(int index);
    void on_comboBox_led_red_currentIndexChanged(int index);
    void on_comboBox_led_ir_currentIndexChanged(int index);
    void on_checkBox_mode_changed(bool checked);
    void on_slider_heart_rate_changed(int value);
    void on_slider_blood_oxygen_changed(int value);

signals:
    void sig_SendControl(const QString &name, const QVariantMap &params);
};

#endif // SENSOR_MAX30102_H
