#pragma once

#include "Order.h"
#include <string>

// ANSI Color Codes from chatgpt
const std::string RESET = "\033[0m";
const std::string BOLD = "\033[1m";
const std::string RED = "\033[1;31m";
const std::string GREEN = "\033[1;32m";
const std::string YELLOW = "\033[1;33m";
const std::string BLUE = "\033[1;34m";
const std::string MAGENTA = "\033[1;35m";
const std::string CYAN = "\033[1;36m";
const std::string WHITE = "\033[1;37m";

class Dashboard {
public:
    /**
     * @brief Clears the terminal and displays the full order book.
     * @param book The OrderBook to display.
     * @param rowsToShow The number of price levels to show on each side.
     */
    void display(const OrderBook& book, int rowsToShow = 15) const;

private:
    /**
     * @brief Clears the terminal screen.
     */
    void clearScreen() const;

    /**
     * @brief Prints a single formatted row.
     */
    void printRow(const std::string& bidQty, const std::string& bidPrice,
                  const std::string& askPrice, const std::string& askQty) const;
};
