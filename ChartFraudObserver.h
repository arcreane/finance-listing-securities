#pragma once
#include "IFraudObserver.h"
#include "multipleViewsWindow.h"
#include <memory>
#include <map>

class ChartFraudObserver : public IFraudObserver {
public:
    ChartFraudObserver(MultipleViewsWindow* mainWindow)
        : m_mainWindow(mainWindow) {
    }

    void update(const Alert& alert) override {
        m_mainWindow->addAlert(alert);
    }

private:
    MultipleViewsWindow* m_mainWindow;
};