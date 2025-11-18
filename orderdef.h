#pragma once

#include <cstdint>


enum class Side {
    BUY,
    SELL
};

//single limit order
struct Order {
    uint64_t id;        // Unique order ID chatgpt
    Side side;          
    double price;       
    int quantity;       
    uint64_t timestamp; 

    
    Order() : id(0), side(Side::BUY), price(0.0), quantity(0), timestamp(0) {}

    
    Order(Side p_side, double p_price, int p_quantity)
        : id(0), side(p_side), price(p_price), quantity(p_quantity), timestamp(0) {}
};
