#ifndef MAX30102_H
#define MAX30102_H

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

private:
    void initUI();
};

#endif // MAX30102_H
