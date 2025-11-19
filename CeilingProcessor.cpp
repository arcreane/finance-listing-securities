#include "CeilingProcessor.h"
#include <chrono>

std::optional<Alert> CeilingProcessor::process(const Order& order)
{
	historicOrders.push_back(order);

	while (!historicOrders.empty() && historicOrders.begin()->getTimestampCreated() < order.getTimestampCreated() - window) {
		historicOrders.erase(historicOrders.begin());
	}
	
	if (order.getPrice() > ceilingPriceHistoric - priceMargin)
	{
		suspiciousOrders.push_back(order);
		int totalQuantity = 0;
		for (Order suspected : suspiciousOrders)
		{
			if(order.getIdFirm() == suspected.getIdFirm())
			{
				totalQuantity += suspected.getQuantity();
			}
		}
		if ((totalQuantity/ceilingVolumeHistoric) > percentageVolumeThreshlod)
		{
			return Alert(order.getId(), this->getId(), FraudType::CEILING, AlertSeverity::HIGH, AlertType::VOLUME, "Ceiling scheme detected", order.getTimestampCreated());
		}
	}

	return std::nullopt;
}