#include "multipleViewsWindow.h"

#include "CeilingProcessor.h"
#include "FloorProcessor.h"
#include "PumpDumpProcessor.h"
#include "SpoofingProcessor.h"
#include "WashProcessor.h"
#include "DbConnector.h"
#include <QApplication>
#include <FraudMonitor.cpp>
#include <ChartFraudObserver.h>
#include <ChartOrderObserver.h>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    // Qt app création
    MultipleViewsWindow mainWindow;
    mainWindow.show();

    FraudMonitor fraudMonitor;

    // Add processors
    fraudMonitor.addProcessor(std::make_shared<CeilingProcessor>(5));
    fraudMonitor.addProcessor(std::make_shared<FloorProcessor>(5));
	fraudMonitor.addProcessor(std::make_shared<PumpDumpProcessor>());
	fraudMonitor.addProcessor(std::make_shared<SpoofingProcessor>());
	//fraudMonitor.addProcessor(std::make_shared<WashProcessor>());

    // Add observers
    std::shared_ptr<ChartFraudObserver> chartFraudObserver = std::make_shared<ChartFraudObserver>(&mainWindow);
	std::shared_ptr<ChartOrderObserver> chartOrderObserver = std::make_shared<ChartOrderObserver>(&mainWindow);
    fraudMonitor.addAlertObserver(chartFraudObserver);
	fraudMonitor.addOrderObserver(chartOrderObserver);
    
	//Orders processing
	DbConnector dbConnector;
	std::vector<Order> orders;

    try
    {
        orders = dbConnector.getOrders("orders.json");
    }
    catch (const std::exception& e)
    {
		cerr << "Error retriving data " << e.what() << endl;
    }
	

    for(Order order : orders)
    {
        fraudMonitor.processOrder(order);
    }

	//mainWindow.addAlert(Alert(1,1, FraudType::PUMP_AND_DUMP, AlertSeverity::HIGH, AlertType::VOLUME, "Test alert", time_t( 1737475044)));

    return app.exec();
}