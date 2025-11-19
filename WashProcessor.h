#pragma once
#include "Order.h"
#include "IProcessor.h"
#include <vector>

class WashProcessor : public IProcessor {
public:
	WashProcessor() {}
	std::optional<Alert> process(const Order& order) override;

private:
	time_t window;
	double volumeHistoric;
	std::vector<Order> suspiciousOrders;
	std::vector<Order> historicOrders;
};