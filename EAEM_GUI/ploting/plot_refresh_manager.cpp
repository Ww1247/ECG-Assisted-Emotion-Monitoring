#include "plot_refresh_manager.h"

PlotRefreshManager::PlotRefreshManager(QObject *parent)
    : QObject(parent),
      refreshTimer_(new QTimer(this))
{
    connect(refreshTimer_, &QTimer::timeout, this, &PlotRefreshManager::onRefreshTick);
    refreshTimer_->start(33);
}

PlotRefreshManager::~PlotRefreshManager()
{
    refreshTimer_->stop();
    plotWidgets_.clear();
}

void PlotRefreshManager::setRefreshInterval(int ms)
{
    refreshTimer_->setInterval(ms);
}

void PlotRefreshManager::registerPlot(PlotRefreshInterface *plot)
{
    if (plot) plotWidgets_.insert(plot);
}

void PlotRefreshManager::unregisterPlot(PlotRefreshInterface *plot)
{
    plotWidgets_.remove(plot);
}

void PlotRefreshManager::onRefreshTick()
{
    for (PlotRefreshInterface *plot : plotWidgets_) {
        if (plot) plot->updatePlot();
    }
}

