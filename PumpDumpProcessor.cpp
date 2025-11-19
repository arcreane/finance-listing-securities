#include "PumpDumpProcessor.h"
#include <chrono>

std::optional<Alert> PumpDumpProcessor::process(const Order& order)
{
    // Ajouter l'ordre à l'historique
    historicOrders.push_back(order);

    // Nettoyer les ordres trop anciens
    while (!historicOrders.empty() && historicOrders.front().getTimestampCreated() < order.getTimestampCreated() - window) {
        historicOrders.erase(historicOrders.begin());
    }

    // Nettoyer également les ordres suspects trop anciens
    auto cleanOldOrders = [this, &order](std::vector<Order>& orders) {
        auto it = std::remove_if(orders.begin(), orders.end(),
            [this, &order](const Order& o) {
                return o.getTimestampCreated() < order.getTimestampCreated() - window;
            });
        orders.erase(it, orders.end());
        };

    cleanOldOrders(suspiciousBuyOrders);
    cleanOldOrders(suspiciousSellOrders);

    // Calculer le prix moyen historique pour cet instrument
    double avgPrice = 0.0;
    int priceCount = 0;
    for (const Order& histOrder : historicOrders) {
        if (histOrder.getIdInstrument() == order.getIdInstrument() &&
            histOrder.getPrice() > 0) {
            avgPrice += histOrder.getPrice();
            priceCount++;
        }
    }

    if (priceCount > 0) {
        avgPrice /= priceCount;
    }

    // Détecter les ordres suspects basés sur le volume et le prix
    const double VOLUME_THRESHOLD = volumeHistoric;
    const double PRICE_INCREASE_THRESHOLD = 1.2; // 20% d'augmentation
    const double PRICE_DECREASE_THRESHOLD = 0.8; // 20% de diminution

    bool isHighVolume = order.getQuantity() > VOLUME_THRESHOLD;
    bool isPriceIncreasing = avgPrice > 0 && order.getPrice() > avgPrice * PRICE_INCREASE_THRESHOLD;
    bool isPriceDecreasing = avgPrice > 0 && order.getPrice() < avgPrice * PRICE_DECREASE_THRESHOLD;

    // Phase d'accumulation (Pump)
    if (isHighVolume && order.getSide() == side::BUY && isPriceIncreasing) {
        suspiciousBuyOrders.push_back(order);
    }

    // Phase de vente (Dump)
    if (isHighVolume && order.getSide() == side::SELL) {
        suspiciousSellOrders.push_back(order);
    }

    // Analyser les patterns pour détecter pump and dump
    double totalBuyQuantity = 0;
    double totalSellQuantity = 0;
    double buyValue = 0;
    double sellValue = 0;

    for (const Order& suspected : suspiciousBuyOrders) {
        if (order.getIdInstrument() == suspected.getIdInstrument()) {
            totalBuyQuantity += suspected.getQuantity();
            buyValue += suspected.getPrice() * suspected.getQuantity();
        }
    }

    for (const Order& suspected : suspiciousSellOrders) {
        if (order.getIdInstrument() == suspected.getIdInstrument()) {
            totalSellQuantity += suspected.getQuantity();
            sellValue += suspected.getPrice() * suspected.getQuantity();
        }
    }

    // Calculer les prix moyens d'achat et de vente
    double avgBuyPrice = (totalBuyQuantity > 0) ? buyValue / totalBuyQuantity : 0;
    double avgSellPrice = (totalSellQuantity > 0) ? sellValue / totalSellQuantity : 0;

    // Vérifier si nous observons un pattern de pump and dump
    const double MIN_BUY_SELL_RATIO = 2.0; // Minimum ratio for detection
    const double SIGNIFICANT_PRICE_DROP = 0.8; // 20% drop from pump to dump

    // Détecter un volume d'achat significatif suivi par des ventes importantes
    if (totalBuyQuantity > VOLUME_THRESHOLD * 3 &&
        totalSellQuantity > VOLUME_THRESHOLD &&
        avgSellPrice < avgBuyPrice * SIGNIFICANT_PRICE_DROP) {

        // Vérifier le ratio temporel (les achats devraient précéder les ventes)
        time_t avgBuyTime = 0, avgSellTime = 0;
        for (const Order& o : suspiciousBuyOrders) {
            if (o.getIdInstrument() == order.getIdInstrument()) {
                avgBuyTime += o.getTimestampCreated();
            }
        }
        for (const Order& o : suspiciousSellOrders) {
            if (o.getIdInstrument() == order.getIdInstrument()) {
                avgSellTime += o.getTimestampCreated();
            }
        }

        if (suspiciousBuyOrders.size() > 0) avgBuyTime /= suspiciousBuyOrders.size();
        if (suspiciousSellOrders.size() > 0) avgSellTime /= suspiciousSellOrders.size();

        // Si en moyenne les achats précèdent les ventes
        if (avgSellTime > avgBuyTime) {
            std::string reason = "Pump and dump detected: high volume buys followed by sells with price drop";
            return Alert(order.getId(), this->getId(), FraudType::PUMP_AND_DUMP,
                AlertSeverity::HIGH, AlertType::PRICE, reason, order.getTimestampCreated());
        }
    }

    return std::nullopt;
}