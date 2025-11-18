#pragma once

#include <queue>
#include <vector>
#include <map>
#include <string>
#include <cstdint>

enum class Side { BUY, SELL };

struct Order {
    uint64_t id;
    Side side;
    double price;
    int quantity;
    uint64_t timestamp;
};

// Comparator for the BUY side (max-heap on price, min-heap on time)
struct BuyOrderComparator {
    bool operator()(const Order& a, const Order& b) const {
        if (a.price != b.price) {
            return a.price < b.price; // Higher price has higher priority
        }
        return a.timestamp > b.timestamp; // Earlier time has higher priority
    }
};

// Comparator for the SELL side (min-heap on price, min-heap on time)
struct SellOrderComparator {
    bool operator()(const Order& a, const Order& b) const {
        if (a.price != b.price) {
            return a.price > b.price; // Lower price has higher priority
        }
        return a.timestamp > b.timestamp; // Earlier time has higher priority
    }
};

class OrderBook {
public:
    OrderBook() : nextOrderId(1) {}

    /**
     * @brief Adds a new order to the book, triggering matching logic.
     * @param order The new order to be added.
     */
    void addOrder(Order order);

    /**
     * @brief Gets the aggregated buy-side price levels for display.
     * @return A const reference to the bidPriceLevels map.
     */
    const std::map<double, int, std::greater<double>>& getBidLevels() const {
        return bidPriceLevels;
    }

    /**
     * @brief Gets the aggregated sell-side price levels for display.
     * @return A const reference to the askPriceLevels map.
     */
    const std::map<double, int, std::less<double>>& getAskLevels() const {
        return askPriceLevels;
    }

    /**
     * @brief Gets the log of recent trades.
     * @return A const reference to the trade log.
     */
    const std::vector<std::string>& getTradeLog() const {
        return tradeLog;
    }

private:
    // Priority queues for matching (price-time priority)
    std::priority_queue<Order, std::vector<Order>, BuyOrderComparator> bids;
    std::priority_queue<Order, std::vector<Order>, SellOrderComparator> asks;

    // Maps for aggregated display (price -> total quantity)
    // std::greater sorts keys from high to low (bids)
    std::map<double, int, std::greater<double>> bidPriceLevels;
    // std::less (default) sorts keys from low to high (asks)
    std::map<double, int, std::less<double>> askPriceLevels;

    std::vector<std::string> tradeLog; // Log of recent trades
    static const size_t MAX_TRADE_LOG = 10; // Max trades to show

    uint64_t nextOrderId; // Counter for unique order IDs

    /**
     * @brief Internal matching logic, called by addOrder.
     * @param newOrder The order to match. Its quantity will be modified.
     */
    void matchOrder(Order& newOrder);

    /**
     * @brief Logs a trade execution.
     */
    void logTrade(uint64_t id1, uint64_t id2, double price, int quantity);
};
