#pragma once
#include "IFraudObserver.h"

class AlertObserver : public IFraudObserver {
public:
	void update(const Alert& alert) override;
};