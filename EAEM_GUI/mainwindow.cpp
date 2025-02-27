#include "mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Set up the main window
    setWindowTitle("Manual UI Example");
    resize(400, 300);

    // Create the button
    button = new QPushButton("Click Me", this);
    button->setGeometry(150, 120, 100, 50);  // Position and size of the button

    // Create a layout
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(button);

    // Create a central widget to hold the layout
    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    // Connect the button's clicked signal to the application's quit function
    connect(button, &QPushButton::clicked, qApp, &QCoreApplication::quit);
}

MainWindow::~MainWindow()
{
    // No need to delete UI here, as it is handled by Qt's parent-child memory management
    qDebug() << "Windows Closed.";
}
