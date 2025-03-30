#ifndef PLOT_REFRESH_INTERFACE_H
#define PLOT_REFRESH_INTERFACE_H
#pragma once

class PlotRefreshInterface {
public:
    virtual ~PlotRefreshInterface() {}
    virtual void updatePlot() = 0;
};

#endif // PLOT_REFRESH_INTERFACE_H
