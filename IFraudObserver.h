#pragma once
#include "Alert.h"

class IFraudObserver {
public:
	virtual void update(const Alert& alert) = 0;
};