/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <qchartview.h>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QHBoxLayout *mainLayout;
    QVBoxLayout *leftLayout;
    QLabel *videoLabel;
    QPushButton *startButton;
    QLabel *resultLabel;
    QVBoxLayout *rightLayout;
    QLabel *rateLabel;
    QLabel *statusLabel;
    QLabel *maxRateLabel;
    QChartView *chartView;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1000, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        mainLayout = new QHBoxLayout(centralwidget);
        mainLayout->setObjectName(QString::fromUtf8("mainLayout"));
        leftLayout = new QVBoxLayout();
        leftLayout->setObjectName(QString::fromUtf8("leftLayout"));
        videoLabel = new QLabel(centralwidget);
        videoLabel->setObjectName(QString::fromUtf8("videoLabel"));
        videoLabel->setMinimumSize(QSize(480, 360));
        videoLabel->setFrameShape(QFrame::Box);
        videoLabel->setAlignment(Qt::AlignCenter);

        leftLayout->addWidget(videoLabel);

        startButton = new QPushButton(centralwidget);
        startButton->setObjectName(QString::fromUtf8("startButton"));

        leftLayout->addWidget(startButton);

        resultLabel = new QLabel(centralwidget);
        resultLabel->setObjectName(QString::fromUtf8("resultLabel"));

        leftLayout->addWidget(resultLabel);


        mainLayout->addLayout(leftLayout);

        rightLayout = new QVBoxLayout();
        rightLayout->setObjectName(QString::fromUtf8("rightLayout"));
        rateLabel = new QLabel(centralwidget);
        rateLabel->setObjectName(QString::fromUtf8("rateLabel"));

        rightLayout->addWidget(rateLabel);

        statusLabel = new QLabel(centralwidget);
        statusLabel->setObjectName(QString::fromUtf8("statusLabel"));

        rightLayout->addWidget(statusLabel);

        maxRateLabel = new QLabel(centralwidget);
        maxRateLabel->setObjectName(QString::fromUtf8("maxRateLabel"));

        rightLayout->addWidget(maxRateLabel);

        chartView = new QChartView(centralwidget);
        chartView->setObjectName(QString::fromUtf8("chartView"));

        rightLayout->addWidget(chartView);


        mainLayout->addLayout(rightLayout);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "\350\241\250\346\203\205\350\257\206\345\210\253 + \345\277\203\347\216\207\347\233\221\346\265\213", nullptr));
        videoLabel->setText(QCoreApplication::translate("MainWindow", "\346\221\204\345\203\217\345\244\264\347\224\273\351\235\242", nullptr));
        startButton->setText(QCoreApplication::translate("MainWindow", "\345\274\200\345\247\213\346\243\200\346\265\213", nullptr));
        resultLabel->setText(QCoreApplication::translate("MainWindow", "10\347\247\222\346\234\200\345\244\232\346\203\205\347\273\252:", nullptr));
        rateLabel->setText(QCoreApplication::translate("MainWindow", "BPM: --", nullptr));
        statusLabel->setText(QCoreApplication::translate("MainWindow", "\347\212\266\346\200\201: --", nullptr));
        maxRateLabel->setText(QCoreApplication::translate("MainWindow", "10\347\247\222\346\234\200\345\244\247\345\277\203\347\216\207: --", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
