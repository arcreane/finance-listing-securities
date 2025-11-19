#pragma once
#include <string>
using namespace std;

enum class AlertType {
	PRICE,
	VOLUME,
	NEWS,
	TECHNICAL
};

enum class AlertSeverity {
	LOW,
	MEDIUM,
	HIGH
};

enum class FraudType {
	WASH,
	SPOOFING,
	PUMP_AND_DUMP,
	QUOTE_STUFFING,
	CEILING,
	FLOOR
};

class Alert {
public:
	Alert(int orderId, int processorId, FraudType fraudType, AlertSeverity severity, AlertType type, const string& description, const time_t& timestamp)
		: _id(getNextId()),
		 _orderId(orderId),
		 _processorId(processorId),
		 _fraudType(fraudType),
		 _severity(severity),
		 _type(type),
		 _description(description),
		 _timestamp(timestamp) {}
	int getId() const { return _id; }
	int getOrderId() const { return _orderId; }
	int getProcessorId() const { return _processorId; }
	FraudType getFraudType() const { return _fraudType; }
	AlertSeverity getSeverity() const { return _severity; }
	AlertType getType() const { return _type; }
	string getDescription() const { return _description; }
	time_t getTimestamp() const { return _timestamp; }

private:
	const int _id;
	int _orderId;	
	int _processorId;
	FraudType _fraudType;
	AlertSeverity _severity;
	AlertType _type;
	std::string _description;
	time_t _timestamp;

	static int getNextId() {
		static int nextId{ 0 };
		return nextId++;
	}
};