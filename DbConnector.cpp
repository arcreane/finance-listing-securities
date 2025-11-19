#include "DbConnector.h"

using json = nlohmann::json;

std::vector<Order> DbConnector::getOrders(const std::string& filename)
{
    // 1. Vérification initiale du fichier
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open JSON file: " + filename);
    }

    // 2. Parsing du JSON avec gestion d'erreur
    json jsonData;
    try {
        file >> jsonData;
    }
    catch (const json::parse_error& e) {
        file.close();
        throw std::runtime_error("JSON parsing error: " + std::string(e.what()));
    }

    // 3. Vérification si jsonData contient la clé "orders" et si c'est un tableau
    if (!jsonData.contains("orders") || !jsonData["orders"].is_array()) {
        file.close();
        throw std::runtime_error("JSON data must contain an 'orders' array");
    }

    // 4. Accès au tableau "orders"
    const auto& ordersArray = jsonData["orders"];

    // 5. Optimisation : réservation de l'espace dans le vector
    std::vector<Order> orders;
    orders.reserve(ordersArray.size());

    for (const auto& orderJson : ordersArray) {
        try {
            orders.push_back(parseOrderFromJson(orderJson));
        }
        catch (const std::exception& e) {
            std::cerr << "Error parsing order: " << e.what() << std::endl;
        }
    }

    file.close();
    return orders;
}

// Pour les enums
timeInForce stringToTimeInForce(const std::string& str) {
    if (str == "GTC") return timeInForce::GTC;
    if (str == "FOK") return timeInForce::FOK;
    if (str == "DAY") return timeInForce::DAY;
    if (str == "IOC") return timeInForce::IOC;
    throw std::invalid_argument("Invalid timeInForce value");
}

orderType stringToOrderType(const std::string& str) {
    if (str == "LIMIT") return orderType::LIMIT;
    if (str == "MARKET") return orderType::MARKET;
    if (str == "STOP") return orderType::STOP;
    if (str == "STOP_LIMIT") return orderType::STOP_LIMIT;
    throw std::invalid_argument("Invalid orderType value");
}

side stringToSide(const std::string& str) {
    if (str == "BUY") return side::BUY;
    if (str == "SELL") return side::SELL;
    throw std::invalid_argument("Invalid side value");
}

status stringToStatus(const std::string& str) {
    if (str == "PENDING" || str == "NEW") return status::PENDING;
    if (str == "CANCELLED") return status::CANCELLED;
    if (str == "EXECUTED") return status::EXECUTED;
    throw std::invalid_argument("Invalid status value");
}

instrumentType stringToInstrumentType(const std::string& str) {
    if (str == "STOCK") return instrumentType::STOCK;
    if (str == "OPTION") return instrumentType::OPTION;
    if (str == "FUTURE") return instrumentType::FUTURE;
    if (str == "BOND") return instrumentType::BOND;
    throw std::invalid_argument("Invalid instrumentType value");
}

// Récup les timestamp
time_t stringToTimestamp(const std::string& str) {
    if (str.empty()) return 0;

    std::tm tm = {};
    std::istringstream ss(str);
    ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
    if (ss.fail()) {
        throw std::invalid_argument("Invalid timestamp format");
    }
    return std::mktime(&tm);
}

Order DbConnector::parseOrderFromJson(const json& jsonData) {
    try {

        int id = std::stoi(jsonData["_id"].get<std::string>().substr(0, 8), nullptr, 16);

        return Order(
            id,
            jsonData["priority"],
            jsonData["price"],
            jsonData["quantity"],
            stringToTimeInForce(jsonData["timeInForce"]),
            stringToOrderType(jsonData["orderType"]),
            stringToSide(jsonData["side"]),
            jsonData["idInstrument"],
            jsonData["originalQty"],
            jsonData["idFirm"],
            jsonData["stopPrice"],
            jsonData["limitPrice"],
            stringToStatus(jsonData["status"]),
            stringToTimestamp(jsonData["timestampCreated"]),
            stringToTimestamp(jsonData["timestampExecuted"]),
            jsonData["remainingQty"],
            jsonData["totalValue"],
            stringToInstrumentType(jsonData["instrumentType"]),
            jsonData["commission"],
            jsonData["marketId"],
            jsonData["origin"],
            stringToTimestamp(jsonData["expiration"])
        );
    }
    catch (const std::exception& e) {
        throw std::runtime_error(std::string("Error parsing order: ") + e.what());
    }
}