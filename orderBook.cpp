#include "Order.h"
#include "utility.h" // For getCurrentTimestamp
#include <iostream>
#include <iomanip> // For std::setprecision
#include <sstream> // For std::stringstream

void OrderBook::addOrder(Order order) {
    // Assign a unique ID and timestamp
    order.id = nextOrderId++;
    order.timestamp = getCurrentTimestamp();

    // 1. Try to match the order
    matchOrder(order);

    // 2. If the order is not fully filled, add it to the book
    if (order.quantity > 0) {
        if (order.side == Side::BUY) {
            bids.push(order);
            bidPriceLevels[order.price] += order.quantity;
        } else {
            asks.push(order);
            askPriceLevels[order.price] += order.quantity;
        }
    }
}

void OrderBook::matchOrder(Order& newOrder) {
    while (newOrder.quantity > 0) {
        if (newOrder.side == Side::BUY) {
            // New order is BUY. Check for matches in ASKS.
            if (asks.empty() || newOrder.price < asks.top().price) {
                // No matching ask (either no asks, or best ask price is too high)
                break;
            }

            // A match is possible!
            // Get the best ask order (lowest price)
            // We need a mutable reference, which is tricky with priority_queue.
            // Standard practice: pop, modify, push back if not fully filled.
            Order bestAsk = asks.top();
            asks.pop();

            // Determine trade quantity
            int tradeQuantity = std::min(newOrder.quantity, bestAsk.quantity);

            // Log the trade
            logTrade(newOrder.id, bestAsk.id, bestAsk.price, tradeQuantity);

            // Update quantities
            newOrder.quantity -= tradeQuantity;
            bestAsk.quantity -= tradeQuantity;

            // Update the aggregated display map
            askPriceLevels[bestAsk.price] -= tradeQuantity;
            if (askPriceLevels[bestAsk.price] <= 0) {
                askPriceLevels.erase(bestAsk.price);
            }

            // If the bestAsk was only partially filled, add it back to the book
            if (bestAsk.quantity > 0) {
                asks.push(bestAsk);
            }

        } else {
            // New order is SELL. Check for matches in BIDS.
            if (bids.empty() || newOrder.price > bids.top().price) {
                // No matching bid (either no bids, or best bid price is too low)
                break;
            }

            // A match is possible!
            Order bestBid = bids.top();
            bids.pop();

            // Determine trade quantity
            int tradeQuantity = std::min(newOrder.quantity, bestBid.quantity);

            // Log the trade (price is from the resting order, i.e., the bid)
            logTrade(newOrder.id, bestBid.id, bestBid.price, tradeQuantity);

            // Update quantities
            newOrder.quantity -= tradeQuantity;
            bestBid.quantity -= tradeQuantity;

            // Update the aggregated display map
            bidPriceLevels[bestBid.price] -= tradeQuantity;
            if (bidPriceLevels[bestBid.price] <= 0) {
                bidPriceLevels.erase(bestBid.price);
            }

            // If the bestBid was only partially filled, add it back
            if (bestBid.quantity > 0) {
                bids.push(bestBid);
            }
        }
    }
}

void OrderBook::logTrade(uint64_t id1, uint64_t id2, double price, int quantity) {
    std::stringstream ss;
    ss << "TRADE: " << quantity << " @ " << std::fixed << std::setprecision(2) << price
       << " (Orders " << id1 << " & " << id2 << ")";
    
    tradeLog.push_back(ss.str());
    if (tradeLog.size() > MAX_TRADE_LOG) {
        tradeLog.erase(tradeLog.begin());
    }
}
