#include "Order.h"
#include "utility.h"
#include <iostream>
#include <iomanip> 
#include <sstream> 


using namespace std;

void OrderBook::addOrder(Order order) {
    // Assign a unique ID and timestamp
    order.id = nextOrderId++;
    order.timestamp = getCurrentTimestamp();

    // match the order
    matchOrder(order);

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
                break;
            }

            // A match psosible!
            
            Order bestAsk = asks.top();
            asks.pop();

            int tradeQuantity = min(newOrder.quantity, bestAsk.quantity); 

            
            logTrade(newOrder.id, bestAsk.id, bestAsk.price, tradeQuantity);

            
            newOrder.quantity -= tradeQuantity;
            bestAsk.quantity -= tradeQuantity;

           
            askPriceLevels[bestAsk.price] -= tradeQuantity;
            if (askPriceLevels[bestAsk.price] <= 0) {
                askPriceLevels.erase(bestAsk.price);
            }

           
            if (bestAsk.quantity > 0) {
                asks.push(bestAsk);
            }

        } else {
            // New order is SELL. Check for matches in BIDS.
            if (bids.empty() || newOrder.price > bids.top().price) {
                
                break;
            }

            // A match posible
            Order bestBid = bids.top();
            bids.pop();

            // Determine trade quantity
            int tradeQuantity = min(newOrder.quantity, bestBid.quantity); 

            
            logTrade(newOrder.id, bestBid.id, bestBid.price, tradeQuantity);

            
            newOrder.quantity -= tradeQuantity;
            bestBid.quantity -= tradeQuantity;

            
            bidPriceLevels[bestBid.price] -= tradeQuantity;
            if (bidPriceLevels[bestBid.price] <= 0) {
                bidPriceLevels.erase(bestBid.price);
            }

            // putit back
            if (bestBid.quantity > 0) {
                bids.push(bestBid);
            }
        }
    }
}

void OrderBook::logTrade(uint64_t id1, uint64_t id2, double price, int quantity) {
    stringstream ss;
    ss << "TRADE: " << quantity << " @ " << fixed << setprecision(2) << price 
       << " (Orders " << id1 << " & " << id2 << ")";
    
    tradeLog.push_back(ss.str());
    if (tradeLog.size() > MAX_TRADE_LOG) {
        tradeLog.erase(tradeLog.begin());
    }
}
