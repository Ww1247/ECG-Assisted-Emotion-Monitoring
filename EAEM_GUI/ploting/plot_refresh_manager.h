#ifndef PLOTREFRESHMANAGER_H
#define PLOTREFRESHMANAGER_H

#include <QObject>
#include <QTimer>
#include <QSet>
#include "plot_refresh_interface.h"

class PlotRefreshManager : public QObject {
    Q_OBJECT
public:
    explicit PlotRefreshManager(QObject *parent = nullptr);
    ~PlotRefreshManager();

    void registerPlot(PlotRefreshInterface *plot);
    void unregisterPlot(PlotRefreshInterface *plot);
    void setRefreshInterval(int ms);

private slots:
    void onRefreshTick();

private:
    QTimer *refreshTimer_;
    QSet<PlotRefreshInterface *> plotWidgets_;
};

#endif // PLOTREFRESHMANAGER_H
