#pragma once
#include "IOrderObserver.h"
#include "multipleViewsWindow.h"
#include <memory>
#include <map>

class ChartOrderObserver : public IOrderObserver {
public:
    ChartOrderObserver(MultipleViewsWindow* mainWindow)
        : m_mainWindow(mainWindow) {
    }

    void update(const Order& order) override {
		m_mainWindow->updatePriceChart(order.getTimestampCreated(), order.getPrice()); // Orders are strange in their configuration, they were passed by dataTeam so I tried to do something clean with it
		m_mainWindow->updateOrderBook(order);
		m_mainWindow->updateRawDataOrder(order);
    }

private:
    MultipleViewsWindow* m_mainWindow;
};