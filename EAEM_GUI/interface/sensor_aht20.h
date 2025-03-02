#ifndef AHT20_H
#define AHT20_H

#include <QWidget>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QSpacerItem>

class SensorAHT20Widget : public QWidget {
    Q_OBJECT
public:
    explicit SensorAHT20Widget(QWidget *parent = nullptr);

private:
    void initUI();
};

#endif // AHT20_H
