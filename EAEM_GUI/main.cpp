#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QMetaType>
#include <QTextCursor>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qRegisterMetaType<QTextCursor>("QTextCursor");
    MainWindow w;
    w.show();
    
    return a.exec();
}
