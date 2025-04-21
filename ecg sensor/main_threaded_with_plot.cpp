
#include "mainwindow_threaded_with_plot.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow w;
    w.setWindowTitle("Heart Rate with IR Waveform");
    w.show();
    return app.exec();
}
