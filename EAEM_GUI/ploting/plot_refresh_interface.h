#ifndef PLOT_REFRESH_INTERFACE_H
#define PLOT_REFRESH_INTERFACE_H
#pragma once

/**
 * @brief Interface for plot refreshable widgets or components.
 *
 * Any class that implements this interface must define the updatePlot() method,
 * which is typically used to refresh or redraw a real-time data plot.
 */
class PlotRefreshInterface {
public:
    /**
     * @brief Virtual destructor.
     */
    virtual ~PlotRefreshInterface() {}

    /**
     * @brief Pure virtual function to update the plot.
     *
     * This function should be implemented by any class that needs to
     * update or refresh plotted data on the screen.
     */
    virtual void updatePlot() = 0;
};


#endif // PLOT_REFRESH_INTERFACE_H
