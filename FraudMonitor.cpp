#include "IFraudObserver.h"
#include "IOrderObserver.h"
#include "IProcessor.h"
#include <vector>
#include <memory>

class FraudMonitor {
public:
	FraudMonitor() {}

	void processOrder(const Order& order) {
		notifyObserversOrder(order);
		for (auto processor : _processors) {
			std::optional<Alert> alert = processor->process(order);
			if (alert.has_value()) {
				notifyObserversAlert(alert.value());
			}
		}
	}
	
	void addProcessor(std::shared_ptr<IProcessor> processor) {
		_processors.push_back(std::move(processor));
	}

	void addAlertObserver(std::shared_ptr<IFraudObserver> observer) {
		_observersAlert.push_back(std::move(observer));
	}

	void addOrderObserver(std::shared_ptr<IOrderObserver> observer) {
		_observersOrder.push_back(std::move(observer));
	}

private:
	void notifyObserversAlert(const Alert& alert) {
		for (auto observer : _observersAlert) {
			observer->update(alert);
		}
	}

	void notifyObserversOrder(const Order& order) {
		for (auto observer : _observersOrder) {
			observer->update(order);
		}
	}

	std::vector<std::shared_ptr<IProcessor>> _processors;
	std::vector<std::shared_ptr<IFraudObserver>> _observersAlert;
	std::vector<std::shared_ptr<IOrderObserver>> _observersOrder;
};