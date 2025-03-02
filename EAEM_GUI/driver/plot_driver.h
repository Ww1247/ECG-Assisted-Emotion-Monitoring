#ifndef PLOT_DRIVER_H
#define PLOT_DRIVER_H

#include <QObject>

class PlotDriver : public QObject
{
    Q_OBJECT
public:
    explicit PlotDriver(QObject *parent = nullptr);

signals:
};

#endif // PLOT_DRIVER_H
