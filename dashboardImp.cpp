#include "Dashboard.h"
#include <iostream>
#include <iomanip> 
#include <sstream> 


using namespace std;

void Dashboard::clearScreen() const {
    // ANSI codes to clear screen and move cursor to top-left Chatgpt
    cout << "\033[2J\033[H" << flush; 
}

void Dashboard::printRow(const string& bidQty, const string& bidPrice,
                         const string& askPrice, const string& askQty) const {
    
    cout << GREEN << setw(10) << right << bidQty << " "
              << setw(10) << right << bidPrice << RESET 
              << " | "
              << RED << setw(10) << left << askPrice << " "
              << setw(10) << left << askQty << RESET
              << endl; 
}

void Dashboard::display(const OrderBook& book, int rowsToShow) const {
    clearScreen();
    cout << BOLD << CYAN << "--- CONTINUOUS LIMIT ORDER BOOK (CLOB) ---" << RESET << endl; 
    cout << "Stock: " << BOLD << WHITE << "GEMINI" << RESET << endl; 
    cout << "------------------------------------------" << endl;

    // Headers
    cout << BOLD << setw(10) << right << "BID QTY" << " " 
              << setw(10) << right << "BID $" << RESET 
              << " | "
              << BOLD << setw(10) << left << "ASK $" << " " 
              << setw(10) << left << "ASK QTY" << RESET 
              << endl; 
    
    cout << "------------------------------------------" << endl;

    // price levels
    const auto& bidLevels = book.getBidLevels();
    const auto& askLevels = book.getAskLevels();

    auto bidIter = bidLevels.begin();
    auto askIter = askLevels.begin();

    stringstream ssBidQty, ssBidPrice, ssAskPrice, ssAskQty;
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
            ssBidPrice << fixed << setprecision(2) << bidIter->first; 
            ++bidIter;
        }

        if (askIter != askLevels.end()) {
            ssAskPrice << fixed << setprecision(2) << askIter->first;
            ssAskQty << askIter->second;
            ++askIter;
        }

        printRow(ssBidQty.str(), ssBidPrice.str(), ssAskPrice.str(), ssAskQty.str());
    }

    cout << "----------------------------------------" << endl;
    if (!bidLevels.empty() && !askLevels.empty()) {
        double bestBid = bidLevels.begin()->first;
        double bestAsk = askLevels.begin()->first;
        double spread = bestAsk - bestBid;
        cout << BOLD << YELLOW << "SPREAD: " << fixed << setprecision(2) << spread << RESET << endl;
    } else {
        cout << BOLD << YELLOW << "SPREAD: N/A (One side is empty)" << RESET << endl; 
    }
    cout << "------------------------------------------" << endl; 

    cout << BOLD << MAGENTA << "--- RECENT TRADES ---" << RESET << endl; 
    const auto& trades = book.getTradeLog();
    for (const auto& trade : trades) {
        cout << WHITE << trade << RESET << endl;
    }
}
