#pragma once
#include <ctime>
#include <string>

enum class timeInForce {
	GTC,
	FOK,
	DAY,
	IOC
};

enum class orderType {
	LIMIT,
	MARKET,
	STOP,
	STOP_LIMIT
};

enum class side {
	BUY,
	SELL
};

enum class status {
	PENDING,
	CANCELLED,
	EXECUTED
};

enum class instrumentType {
	STOCK,
	OPTION,
	FUTURE,
	BOND
};


class Order {
private:
	int _id;
	int _priority;
	double _price;
	int _quantity;
	timeInForce _timeInForce;
	orderType _orderType;
	side _side;
	int _idInstrument;
	int _originalQuantity;
	int _idFirm;
	double _stopPrice;
	double _limitPrice;
	status _status;
	time_t _timestampCreated;
	time_t _timestampExecuted;
	int _remainingQuantity;
	double _totalvalue;
	instrumentType _instrumentType;
	double _commission;
	std::string _marketId;
	std::string _origin;
	time_t expiration;

public:
	Order(int id, int priority, double price, int quantity, timeInForce tif, orderType ot, side s, int idInstrument, 
       int originalQuantity, int idFirm, double stopPrice, double limitPrice, status st, time_t timestampCreated, 
       time_t timestampExecuted, int remainingQuantity, double totalValue, instrumentType it, double commission, 
       const std::string& marketId, const std::string& origin, time_t exp)
		: _id(id), _priority(priority), _price(price), _quantity(quantity), _timeInForce(tif), _orderType(ot), _side(s), 
    _idInstrument(idInstrument), _originalQuantity(originalQuantity), _idFirm(idFirm), _stopPrice(stopPrice), 
    _limitPrice(limitPrice), _status(st), _timestampCreated(timestampCreated), _timestampExecuted(timestampExecuted), 
    _remainingQuantity(remainingQuantity), _totalvalue(totalValue), _instrumentType(it), _commission(commission), 
    _marketId(marketId), _origin(origin), expiration(exp) {}

	int getId() const { return _id; }
	int getPriority() const { return _priority; }
	double getPrice() const { return _price; }
	int getQuantity() const { return _quantity; }
	timeInForce getTimeInForce() const { return _timeInForce; }
	orderType getOrderType() const { return _orderType; }
	side getSide() const { return _side; }
	int getIdInstrument() const { return _idInstrument; }
	int getOriginalQuantity() const { return _originalQuantity; }
	int getIdFirm() const { return _idFirm; }
	double getStopPrice() const { return _stopPrice; }
	double getLimitPrice() const { return _limitPrice; }
	status getStatus() const { return _status; }
	time_t getTimestampCreated() const { return _timestampCreated; }
	time_t getTimestampExecuted() const { return _timestampExecuted; }
	int getRemainingQuantity() const { return _remainingQuantity; }
	double getTotalValue() const { return _totalvalue; }
	instrumentType getInstrumentType() const { return _instrumentType; }
	double getCommission() const { return _commission; }
	std::string getMarketId() const { return _marketId; }
	std::string getOrigin() const { return _origin; }
	time_t getExpiration() const { return expiration; }

	// Setters
	void setPriority(int priority) { _priority = priority; }
	void setPrice(double price) { _price = price; }
	void setQuantity(int quantity) { _quantity = quantity; }
	void setTimeInForce(timeInForce tif) { _timeInForce = tif; }
	void setOrderType(orderType ot) { _orderType = ot; }
	void setSide(side s) { _side = s; }
	void setIdInstrument(int idInstrument) { _idInstrument = idInstrument; }
	void setOriginalQuantity(int originalQuantity) { _originalQuantity = originalQuantity; }
	void setIdFirm(int idFirm) { _idFirm = idFirm; }
	void setStopPrice(double stopPrice) { _stopPrice = stopPrice; }
	void setLimitPrice(double limitPrice) { _limitPrice = limitPrice; }
	void setStatus(status st) { _status = st; }
	void setTimestampCreated(time_t timestampCreated) { _timestampCreated = timestampCreated; }
	void setTimestampExecuted(time_t timestampExecuted) { _timestampExecuted = timestampExecuted; }
	void setRemainingQuantity(int remainingQuantity) { _remainingQuantity = remainingQuantity; }
	void setTotalValue(double totalValue) { _totalvalue = totalValue; }
	void setInstrumentType(instrumentType it) { _instrumentType = it; }
	void setCommission(double commission) { _commission = commission; }
	void setMarketId(const std::string& marketId) { _marketId = marketId; }
	void setExpiration(time_t exp) { expiration = exp; }
};