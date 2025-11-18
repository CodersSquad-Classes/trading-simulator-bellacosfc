DOCS: Continuous Limit Order Book (CLOB)

1. Introduction

A stock exchange is a marketplace where stocks and bonds are bought and sold. In every modern electronic exchange there's a "matching engine." The matching engine's job is to process incoming orders from traders and execute trades when a buy order is compatible with a sell order.

The most common system for this is the Continuous Limit Order Book (CLOB). It's a transparent system that maintains a central list of all open buy (bid) and sell (ask) orders. This project implements a simplified, high-performance CLOB in C++ with a real-time terminal dashboard. The engine accepts dynamic orders, matches them based on strict price-time priority, and displays the current market depth.

2. Research on Stock Exchanges

A CLOB operates on a fundamental principle: price-time priority.

Price Priority: This is the first and most important rule. Buyers want the lowest price possible, and sellers want the highest price possible. However, to make a trade, a buyer must meet a seller's price.

Bids (Buy Orders): The highest-priced bid has the highest priority. 
Asks (Sell Orders): The lowest-priced ask has the highest priority. 

Time Priority: This is the tie-breaker. If two orders are at the same price, the one that was placed first gets priority.

The "Spread":
The difference between the highest bid and the lowest ask is called spread.

Best Bid: $101.00

Best Ask: $101.05

Spread: $0.05

A trade only occurs when the spread is zero or negative. This happens in two ways:

A market order arrives. This is an order to "buy at any price" or "sell at any price." A market buy order will immediately match with the best ask ($101.05), whatever it may be.

Our implementation focuses on limit orders, which form the "book." A marketable limit order is one that can be immediately matched. An order that cannot be matched rests in the book, contributing to the market's "depth."

3. Software Design and Implementation

System Architecture

We designed a modular system with clear separation of concerns. The three main components are:

Order: A simple data structure (struct) that holds all information about a single order.

OrderBook: A class that contains all the core logic. It holds the data structures for bids and asks, and contains the addOrder method which performs the matching.

Dashboard: A class responsible only for rendering the state of the OrderBook to the terminal.

main.cpp: The driver program. It initializes the OrderBook and Dashboard, then runs a simulation loop to dynamically create new orders.

Core Data Structures

The key challenge is that the matching engine and the dashboard have different needs.

Matching Engine: Needs to find the best bid and best ask very quickly.

Dashboard: Needs to display all price levels and the total quantity at each level.

We solved this by using a hybrid approach within the OrderBook class.

For Matching (Price-Time Priority): std::priority_queue
We use two priority queues, as suggested:

bids: A max-heap (highest price first) for buy orders.

asks: A min-heap (lowest price first) for sell orders.

We defined custom comparator structs (BuyOrderComparator, SellOrderComparator) that correctly implement the price-time priority logic.

For Display (Aggregated Price Levels): std::map
To efficiently display the dashboard (like the Island example), we also maintain two maps:

bidPriceLevels: A map of price -> total_quantity, sorted from highest to lowest price.

askPriceLevels: A map of price -> total_quantity, sorted from lowest to highest price.

These two sets of data structures are kept in sync on every operation (add, match, or remove order), ensuring both matching and display are fast.

Key Functions

OrderBook::addOrder(Order newOrder)

This is the main entry point for the entire system.

Pseudocode:

function addOrder(newOrder):
  // 1. Give the order a unique ID and timestamp
  newOrder.id = nextOrderId++
  newOrder.timestamp = getCurrentTimestamp()

  // 2. Try to match the order immediately
  matchOrder(newOrder)

  // 3. If the order was not fully filled, add it to the book
  if newOrder.quantity > 0:
    if newOrder.side == BUY:
      bids.push(newOrder)
      bidPriceLevels[newOrder.price] += newOrder.quantity
    else:
      asks.push(newOrder)
      askPriceLevels[newOrder.price] += newOrder.quantity


OrderBook::matchOrder(Order& newOrder)

This is the core matching logic. It's a while loop that continues as long as the new order can be matched with orders already in the book.

Pseudocode:

    function matchOrder(newOrder):
      // Loop while the new order has quantity and is "marketable"
      while newOrder.quantity > 0:
        if newOrder.side == BUY:
          // Check if there are any sellers and if the new buy price
          // is high enough to meet the best (lowest) seller's price
          if asks.empty() or newOrder.price < asks.top().price:
            break // No match possible

      bestAsk = asks.top()
      asks.pop() // Temporarily remove from book

      // ... (Execute Trade) ...
      // ... (See logic for partial fills) ...

      // If bestAsk was only partially filled, add it back
      if bestAsk.quantity > 0:
        asks.push(bestAsk)

    else: // newOrder.side == SELL
      // Check if there are any buyers and if the new sell price
      // is low enough to meet the best (highest) buyer's price
      if bids.empty() or newOrder.price > bids.top().price:
        break // No match possible
      
      bestBid = bids.top()
      bids.pop() // Temporarily remove from book

      // ... (Execute Trade) ...
      // ... (See logic for partial fills) ...

      // If bestBid was only partially filled, add it back
      if bestBid.quantity > 0:
        bids.push(bestBid)



Code Snippet: Trade Execution Logic
This snippet shows how a trade is executed and how partial fills are handled.

// Inside matchOrder, assuming newOrder is BUY and bestAsk is a match
Order& bestAsk = const_cast<Order&>(asks.top());

// Determine trade quantity
int tradeQuantity = std::min(newOrder.quantity, bestAsk.quantity);

// Update quantities
newOrder.quantity -= tradeQuantity;
bestAsk.quantity -= tradeQuantity;

// IMPORTANT: Update the aggregated display map
askPriceLevels[bestAsk.price] -= tradeQuantity;
if (askPriceLevels[bestAsk.price] <= 0) {
    askPriceLevels.erase(bestAsk.price);
}

// Log the trade
logTrade(newOrder.id, bestAsk.id, bestAsk.price, tradeQuantity);

// Remove the (now empty) bestAsk from the book
asks.pop();

// If the bestAsk was only partially filled, add it back
if (bestAsk.quantity > 0) {
    asks.push(bestAsk);
    // The map was already updated, so we don't need to re-add
}


Dashboard::display(const OrderBook& book)

This function uses ANSI escape codes to create a "realistic" dashboard.

\033[2J\033[H: Clears the entire terminal screen and moves the cursor to the top-left (home) position.

It then uses color codes (e.g., \033[1;32m for bright green) to print the headers and data.

It fetches the bidPriceLevels and askPriceLevels maps from the OrderBook.

It iterates through a fixed number of rows (e.g., 15) to display the market depth, padding with empty lines if the book is not that deep.

4. Conclusions and Learnings

(Here are some sample conclusions. You and your teammates should replace these with your own!)

Teammate 1: I learned that the choice of data structure is critical for performance. Using only a priority_queue would make displaying the aggregated book very slow (O(N log N)), and using only a map would make finding the next best order (if the top one is filled) slow. The hybrid approach of using both and keeping them in sync provides the best of both worlds: O(log N) for matching and O(M) for display (where M is display rows).

Teammate 2: I learned about the importance of handling edge cases in the matching logic. The "partial fill" scenario (where an order is only partially matched) is complex. You have to correctly update the order's remaining quantity, update the aggregated map, and then re-insert the partially filled order into the priority queue. Forgetting any of these steps breaks the engine's state.

Teammate 3: I found the "real-time" aspect challenging. Our main.cpp loop with a std::this_thread::sleep_for is a simulation. In a real system, this would be a multi-threaded server, with network I/O handling incoming orders on separate threads and a single, dedicated matching thread (or multiple threads, one per stock) to ensure orders are processed sequentially and fairly. Our implementation highlights the core logic, but a real-world system would need complex concurrency controls.
