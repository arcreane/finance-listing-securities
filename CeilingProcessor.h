#pragma once
#include "Order.h"
#include "IProcessor.h"
#include <vector>

class CeilingProcessor : public IProcessor{
public:
	CeilingProcessor(double priceMargin) : priceMargin(priceMargin) {}
	std::optional<Alert> process(const Order& order) override;

private:
	time_t window;
	double ceilingPriceHistoric;
	double ceilingVolumeHistoric;
	double priceMargin;
	double percentageVolumeThreshlod;
	std::vector<Order> suspiciousOrders;
	std::vector<Order> historicOrders;
};