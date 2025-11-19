#pragma once
#include "Order.h"
#include "Alert.h"
#include <optional>

class IProcessor {
public:
    IProcessor() : _id(getNextId()) {}

    virtual std::optional<Alert> process(const Order& order) = 0;

    int getId() const { return _id; }

private:
    const int _id;

    static int getNextId() {
        static int nextId{ 0 };
        return nextId++;
    }
};