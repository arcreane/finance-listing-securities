#pragma once
#include "Order.h"
#include "IProcessor.h"

//Not in use for now since really close scheme to spoofing scheme
class QuoteProcessor : public IProcessor {
public:
	std::optional<Alert> process(const Order& order) override;
};