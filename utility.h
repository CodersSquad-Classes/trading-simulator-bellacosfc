#pragma once

#include <chrono>
#include <cstdint>
#include <random>
#include "Order.h"

using namespace std;

/**
 * @brief Gets the current time as microseconds since the epoch.
 * Used for order timestamping.
 * @return Current timestamp in microseconds.
 */
inline uint64_t getCurrentTimestamp() {
    return chrono::duration_cast<chrono::microseconds>(
        chrono::system_clock::now().time_since_epoch()
    ).count();
}

/**
 * @brief A simple utility class to generate random orders for simulation.
 */
class OrderGenerator {
private:
    mt19937 gen;
    uniform_int_distribution<> sideDist;
    uniform_int_distribution<> quantityDist;
    normal_distribution<> priceDist; 
    double midPrice;

public:
    OrderGenerator(double startPrice)
        : gen(random_device{}()), 
          sideDist(0, 1), 
          quantityDist(1, 100), 
          priceDist(startPrice, 1.5), 
          midPrice(startPrice) {}

    /**
     * @brief Generates a new random order.
     * @return A new Order object.
     */
    Order generateOrder() {
        Side side = (sideDist(gen) == 0) ? Side::BUY : Side::SELL;
        
        // Generate a price around the midPrice
        double price = round(priceDist(gen) * 100.0) / 100.0;

        
        if (side == Side::BUY) {
            price = round( (midPrice - abs(priceDist(gen) - midPrice)) * 100.0) / 100.0;
        } else {
            price = round( (midPrice + abs(priceDist(gen) - midPrice)) * 100.0) / 100.0;
        }

        int quantity = quantityDist(gen);

        return Order(side, price, quantity);
    }
};
