#pragma once

#include <chrono>
#include <cstdint>
#include <random>
#include "Order.h"

/**
 * @brief Gets the current time as microseconds since the epoch.
 * Used for order timestamping.
 * @return Current timestamp in microseconds.
 */
inline uint64_t getCurrentTimestamp() {
    return std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();
}

/**
 * @brief A simple utility class to generate random orders for simulation.
 */
class OrderGenerator {
private:
    std::mt19937 gen;
    std::uniform_int_distribution<> sideDist;
    std::uniform_int_distribution<> quantityDist;
    std::normal_distribution<> priceDist; // Use normal dist for more realistic prices
    double midPrice;

public:
    OrderGenerator(double startPrice)
        : gen(std::random_device{}()), 
          sideDist(0, 1), 
          quantityDist(1, 100), 
          priceDist(startPrice, 1.5), // (mean, stddev)
          midPrice(startPrice) {}

    /**
     * @brief Generates a new random order.
     * @return A new Order object.
     */
    Order generateOrder() {
        Side side = (sideDist(gen) == 0) ? Side::BUY : Side::SELL;
        
        // Generate a price around the midPrice
        double price = std::round(priceDist(gen) * 100.0) / 100.0; // Round to 2 decimal places

        // Ensure buy orders are slightly below mid and sell orders are slightly above
        // This creates a more realistic book with a spread
        if (side == Side::BUY) {
            price = std::round( (midPrice - std::abs(priceDist(gen) - midPrice)) * 100.0) / 100.0;
        } else {
            price = std::round( (midPrice + std::abs(priceDist(gen) - midPrice)) * 100.0) / 100.0;
        }

        int quantity = quantityDist(gen);

        return Order(side, price, quantity);
    }
};
