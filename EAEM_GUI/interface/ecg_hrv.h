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

class EcgHrvWidget : public QWidget {
    Q_OBJECT
public:
    explicit EcgHrvWidget(QWidget *parent = nullptr);

private:
    void initUI();
};

#endif // ECG_HRV_H
