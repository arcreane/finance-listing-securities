#pragma once
#include "Order.h"
#include "IProcessor.h"
#include <vector>

class PumpDumpProcessor : public IProcessor {
public:
	PumpDumpProcessor() {}
	std::optional<Alert> process(const Order& order) override;

private:
	time_t window;
	double volumeHistoric;
	std::vector<Order> suspiciousBuyOrders;
	std::vector<Order> suspiciousSellOrders;
	std::vector<Order> historicOrders;
};