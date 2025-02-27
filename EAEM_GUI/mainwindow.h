#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>  // Include necessary widgets
#include <QVBoxLayout>  // Layout manager
#include <QApplication>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QPushButton *button;  // Declare a button widget
};

#endif // MAINWINDOW_H
