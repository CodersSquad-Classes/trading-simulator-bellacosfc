#pragma once

#include <cstdint>

// Enum to define the side of the order
enum class Side {
    BUY,
    SELL
};

// Represents a single limit order
struct Order {
    uint64_t id;        // Unique order ID
    Side side;          // BUY or SELL
    double price;       // Limit price
    int quantity;       // Number of shares
    uint64_t timestamp; // Time of order placement (for time priority)

    // Default constructor
    Order() : id(0), side(Side::BUY), price(0.0), quantity(0), timestamp(0) {}

    // Parameterized constructor
    Order(Side p_side, double p_price, int p_quantity)
        : id(0), side(p_side), price(p_price), quantity(p_quantity), timestamp(0) {}
};
