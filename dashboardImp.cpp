#include "Dashboard.h"
#include <iostream>
#include <iomanip> // For std::setw, std::setprecision
#include <sstream> // For std::stringstream

void Dashboard::clearScreen() const {
    // ANSI codes to clear screen and move cursor to top-left
    std::cout << "\033[2J\033[H" << std::flush;
}

void Dashboard::printRow(const std::string& bidQty, const std::string& bidPrice,
                         const std::string& askPrice, const std::string& askQty) const {
    
    std::cout << GREEN << std::setw(10) << std::right << bidQty << " "
              << std::setw(10) << std::right << bidPrice << RESET
              << " | "
              << RED << std::setw(10) << std::left << askPrice << " "
              << std::setw(10) << std::left << askQty << RESET
              << std::endl;
}

void Dashboard::display(const OrderBook& book, int rowsToShow) const {
    clearScreen();
    std::cout << BOLD << CYAN << "--- CONTINUOUS LIMIT ORDER BOOK (CLOB) ---" << RESET << std::endl;
    std::cout << "Stock: " << BOLD << WHITE << "GEMINI" << RESET << std::endl;
    std::cout << "------------------------------------------" << std::endl;

    // Headers
    std::cout << BOLD << std::setw(10) << std::right << "BID QTY" << " "
              << std::setw(10) << std::right << "BID $" << RESET
              << " | "
              << BOLD << std::setw(10) << std::left << "ASK $" << " "
              << std::setw(10) << std::left << "ASK QTY" << RESET
              << std::endl;
    
    std::cout << "------------------------------------------" << std::endl;

    // Get the aggregated price levels
    const auto& bidLevels = book.getBidLevels();
    const auto& askLevels = book.getAskLevels();

    // Use iterators to print side-by-side
    auto bidIter = bidLevels.begin();
    auto askIter = askLevels.begin();

    std::stringstream ssBidQty, ssBidPrice, ssAskPrice, ssAskQty;
    ssBidQty.str("");
    ssBidPrice.str("");
    ssAskPrice.str("");
    ssAskQty.str("");

    for (int i = 0; i < rowsToShow; ++i) {
        ssBidQty.str("");
        ssBidPrice.str("");
        ssAskPrice.str("");
        ssAskQty.str("");

        if (bidIter != bidLevels.end()) {
            ssBidQty << bidIter->second;
            ssBidPrice << std::fixed << std::setprecision(2) << bidIter->first;
            ++bidIter;
        }

        if (askIter != askLevels.end()) {
            ssAskPrice << std::fixed << std::setprecision(2) << askIter->first;
            ssAskQty << askIter->second;
            ++askIter;
        }

        printRow(ssBidQty.str(), ssBidPrice.str(), ssAskPrice.str(), ssAskQty.str());
    }

    // --- SPREAD ---
    std::cout << "------------------------------------------" << std::endl;
    if (!bidLevels.empty() && !askLevels.empty()) {
        double bestBid = bidLevels.begin()->first;
        double bestAsk = askLevels.begin()->first;
        double spread = bestAsk - bestBid;
        std::cout << BOLD << YELLOW << "SPREAD: " << std::fixed << std::setprecision(2) << spread << RESET << std::endl;
    } else {
        std::cout << BOLD << YELLOW << "SPREAD: N/A (One side is empty)" << RESET << std::endl;
    }
    std::cout << "------------------------------------------" << std::endl;

    // --- RECENT TRADES ---
    std::cout << BOLD << MAGENTA << "--- RECENT TRADES ---" << RESET << std::endl;
    const auto& trades = book.getTradeLog();
    for (const auto& trade : trades) {
        std::cout << WHITE << trade << RESET << std::endl;
    }
}
