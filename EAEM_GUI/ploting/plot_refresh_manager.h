#ifndef PLOTREFRESHMANAGER_H
#define PLOTREFRESHMANAGER_H

#include <QObject>
#include <QTimer>
#include <QSet>
#include "plot_refresh_interface.h"

/**
 * @brief The PlotRefreshManager class provides a centralized timer-based manager
 * to periodically update plots implementing the PlotRefreshInterface.
 *
 * This class maintains a set of registered plot widgets and refreshes them at
 * a user-defined interval using a shared QTimer.
 */
class PlotRefreshManager : public QObject {
    Q_OBJECT

public:
    /**
     * @brief Constructs the PlotRefreshManager.
     * @param parent The parent QObject.
     */
    explicit PlotRefreshManager(QObject *parent = nullptr);

    /**
     * @brief Destructor.
     */
    ~PlotRefreshManager();

    /**
     * @brief Registers a widget that implements PlotRefreshInterface for periodic updates.
     * @param plot Pointer to the plot object to register.
     */
    void registerPlot(PlotRefreshInterface *plot);

    /**
     * @brief Unregisters a previously registered plot widget.
     * @param plot Pointer to the plot object to remove.
     */
    void unregisterPlot(PlotRefreshInterface *plot);

    /**
     * @brief Sets the interval at which plots are refreshed.
     * @param ms Refresh interval in milliseconds.
     */
    void setRefreshInterval(int ms);

private slots:
    /**
     * @brief Slot called on each timer tick to refresh all registered plots.
     */
    void onRefreshTick();

private:
    QTimer *refreshTimer_;                      ///< Timer used for triggering refreshes.
    QSet<PlotRefreshInterface *> plotWidgets_;  ///< Set of registered plot widgets to refresh.
};

#endif // PLOTREFRESHMANAGER_H
