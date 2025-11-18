#pragma once

#include <queue>
#include <vector>
#include <map>
#include <string>
#include <cstdint>

using namespace std;

enum class Side { BUY, SELL };

struct Order {
    uint64_t id;
    Side side;
    double price;
    int quantity;
    uint64_t timestamp;

    Order(Side p_side, double p_price, int p_quantity)
        : id(0), side(p_side), price(p_price), quantity(p_quantity), timestamp(0) {}
};

// Comparator for the BUY side (max-heap on price, min-heap on time)
struct BuyOrderComparator {
    bool operator()(const Order& a, const Order& b) const {
        if (a.price != b.price) {
            return a.price < b.price; 
        }
        return a.timestamp > b.timestamp;
    }
};

// Comparator for the SELL side (min-heap on price, min-heap on time)
struct SellOrderComparator {
    bool operator()(const Order& a, const Order& b) const {
        if (a.price != b.price) {
            return a.price > b.price; 
        }
        return a.timestamp > b.timestamp;
    }
};

class OrderBook {
public:
    OrderBook() : nextOrderId(1) {}

    /** Reference from chatgpt:
     * @brief Adds a new order to the book, triggering matching logic.
     * @param order The new order to be added.
     */
    void addOrder(Order order);

    /**
     * @brief Gets the aggregated buy-side price levels for display.
     * @return A const reference to the bidPriceLevels map.
     */
    const map<double, int, greater<double>>& getBidLevels() const {
        return bidPriceLevels;
    }

    /**
     * @brief Gets the aggregated sell-side price levels for display.
     * @return A const reference to the askPriceLevels map.
     */
    const map<double, int, less<double>>& getAskLevels() const {
        return askPriceLevels;
    }

    /**
     * @brief 
     * @return 
     */
    const vector<string>& getTradeLog() const {
        return tradeLog;
    }

private:
    priority_queue<Order, vector<Order>, BuyOrderComparator> bids;
    priority_queue<Order, vector<Order>, SellOrderComparator> asks;

    map<double, int, greater<double>> bidPriceLevels;
    
    map<double, int, less<double>> askPriceLevels;

    vector<string> tradeLog;
    static const size_t MAX_TRADE_LOG = 10; 

    uint64_t nextOrderId;

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
