#include "mainwindow.h"
#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Setting the geometric properties of the main window
    setGeometry(0, 0, 833, 1212);
    setWindowTitle("EAEM_GUI");

    // Creating Centre Controls
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Creating a central layout
    QGridLayout *centralLayout = new QGridLayout(centralWidget);

    // Creating a Connection Group Box
    QGroupBox *groupBoxConnection = new QGroupBox("Connection", centralWidget);
    QVBoxLayout *connectionLayout = new QVBoxLayout(groupBoxConnection);
    QGridLayout *connectionGridLayout = new QGridLayout();

    // Creating a QStackedWidget for connection method switching
    QStackedWidget *stackedWidgetConnectMethod = new QStackedWidget(groupBoxConnection);
    stackedWidgetConnectMethod->setCurrentIndex(0);


    // // Set up the main window
    // setWindowTitle("Manual UI Example");
    // resize(400, 300);

    // // Create the button
    // button = new QPushButton("Click Me", this);
    // button->setGeometry(150, 120, 100, 50);  // Position and size of the button

    // // Create a layout
    // QVBoxLayout *layout = new QVBoxLayout();
    // layout->addWidget(button);

    // // Create a central widget to hold the layout
    // QWidget *centralWidget = new QWidget(this);
    // centralWidget->setLayout(layout);
    // setCentralWidget(centralWidget);

    // // Connect the button's clicked signal to the application's quit function
    // connect(button, &QPushButton::clicked, qApp, &QCoreApplication::quit);
}

MainWindow::~MainWindow()
{
    // No need to delete UI here, as it is handled by Qt's parent-child memory management
    qDebug() << "Windows Closed.";
}
