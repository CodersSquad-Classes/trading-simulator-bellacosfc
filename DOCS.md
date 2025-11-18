#  1. Introduction

A stock exchange is basically a system where people buy and sell stocks. Behind every modern exchange, there is a program called a matching engine. Its job is simple: when someone wants to buy and someone else wants to sell at compatible prices, it matches them and executes a trade.

One of the most common ways to organize these orders is the Continuous Limit Order Book (CLOB). You can think of it as a list that keeps track of all the active buy orders and sell orders. Our project is a simplified version of a CLOB built in C++, with a terminal dashboard that shows what is happening in real time.

The program accepts new orders, checks if they can be matched, and updates the market display accordingly.

#  2. Understanding How a CLOB Works

The CLOB follows one main rule: price-time priority.

*Price Priority*
Buyers want the lowest price; sellers want the highest price. To make a trade, the buyer must accept the seller’s price.

For buy orders (bids), the highest price has priority.
For sell orders (asks), the lowest price has priority.

*Time Priority*

If two orders have the same price, the one that arrived first is taken first.

*The Spread*

The spread is the difference between the best buyer and the best seller.

Example:
Best bid: 101.00
Best ask: 101.05
Spread: 0.05

Trades only happen when one side is willing to meet the other’s price. A market order does this automatically by accepting whatever price is available. Our system focuses on limit orders, but if a limit order can be executed immediately, it is matched right away.

If a limit order cannot be matched, it stays in the book and contributes to the market depth.

#  3. System Design (Explained Simply)

Our project has four main components.

  1. Order

  A small struct that stores basic information about a single order.

  2. OrderBook

  This is the core of the program. It: 
  *Stores all the active buy and sell orders
  *Decides when orders match
  *Updates internal data structures
  *Keeps everything consistent for the dashboard

  3. Dashboard

  This part only deals with displaying the order book in the terminal using colors and formatting.

  4. main.cpp

  Runs a simulation loop that creates orders and feeds them into the OrderBook, then updates the dashboard.

#  Data Structures Used

The matching engine needs fast access to the best prices, while the dashboard needs the data sorted nicely by price levels. To achieve both, we used two different sets of structures.

For fast matching: std::priority_queue
We use:
*A max-heap for bids (highest price first)
*A min-heap for asks (lowest price first)
Custom comparator structs ensure that orders respect price-time priority.

For clean display: std::map
We use two maps to show aggregated quantities:
*bidPriceLevels: price → total quantity
*askPriceLevels: price → total quantity

These maps are sorted automatically, which makes the dashboard simple to render.

Both the queues and maps are updated together every time something changes, so they always stay in sync.

#  Key Functions
OrderBook::addOrder

This runs whenever a new order arrives.
Basic steps:

1. Assign the order an ID and timestamp.
2. Try to match it with the best possible order from the opposite side.
3. If there is leftover quantity, add it to the book.

OrderBook::matchOrder

This function handles all matching logic. It keeps matching as long as:

* The new order has remaining quantity
* The prices on both sides allow a trade

The function removes the best opposing order, performs the trade, adjusts both quantities, updates the maps, and re-inserts partially filled orders if necessary.

Trade Execution Logic

A trade uses:

* The seller’s price
* The minimum of the two order quantities
After updating both orders, the price level maps are also updated. If an order is completely filled, it is removed; if not, it is put back.

#  Dashboard

The dashboard clears the screen using ANSI escape codes and prints a simple table of bid and ask levels. It reads the maps from the OrderBook and shows a fixed number of price levels for consistency.

#  4. Conclusions and Learnings 

Carlos Gouyonnet:
I learned that choosing the right data structures makes a huge difference. Using only priority queues or only maps would make certain operations too slow. Using both together gave us speed and memory efficiency.

Emiliano Espinoza:
I learned that handling partial fills is trickier than it looks. You need to carefully update the remaining quantity, update the maps, and re-insert the order if it is not fully filled. Missing any of these steps can break the entire state of the engine.
