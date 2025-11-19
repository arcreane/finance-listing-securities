#pragma once
#include "Order.h"

class IOrderObserver {
public:
	virtual void update(const Order& order) = 0;
};