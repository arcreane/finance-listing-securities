#pragma once
#include "Order.h"
#include "IProcessor.h"
#include <vector>

class FloorProcessor : public IProcessor {
public:
	FloorProcessor(double priceMargin) : priceMargin(priceMargin) {}
	std::optional<Alert> process(const Order& order) override;

private:
	time_t window;
	double floorPriceHistoric;
	double floorVolumeHistoric;
	double priceMargin;
	double percentageVolumeThreshlod;
	std::vector<Order> suspiciousOrders;
	std::vector<Order> historicOrders;
};