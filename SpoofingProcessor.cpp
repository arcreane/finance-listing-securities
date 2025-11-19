#include "SpoofingProcessor.h"

std::optional<Alert> SpoofingProcessor::process(const Order& order)
{
	historicOrders.push_back(order);
	while (!historicOrders.empty() && historicOrders.begin()->getTimestampCreated() < order.getTimestampCreated() - window) {
		historicOrders.erase(historicOrders.begin());
	}
	/*
	if (order.getQuantity() > volumeHistoric)
	{
		if (order.getSide() == side::BUY)
		{
			suspiciousBuyOrders.push_back(order);
		}
		else
		{
			suspiciousSellOrders.push_back(order);
		}
		int totalBuyQuantity = 0;
		int totalSellQuantity = 0;
		for (Order suspected : suspiciousBuyOrders)
		{
			if (order.getIdFirm() == suspected.getIdFirm())
			{
				totalBuyQuantity += suspected.getQuantity();
			}
		}
		for (Order suspected : suspiciousSellOrders)
		{
			if (order.getIdFirm() == suspected.getIdFirm())
			{
				totalSellQuantity += suspected.getQuantity();
			}
		}
		if ((totalBuyQuantity / totalSellQuantity) > percentageVolumeThreshlod)
		{
			return Alert(order.getId(), this->getId(), FraudType::SPOOFING, AlertSeverity::HIGH, AlertType::VOLUME, "Spoofing scheme detected");
		}
	}
	*/
	return std::nullopt;
}