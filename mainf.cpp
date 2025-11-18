#include "orderdef.h"
#include "dashboard.h"
#include "utility.h"
#include <iostream>
#include <thread>         // For std::this_thread::sleep_for
#include <chrono>         // For std::chrono::milliseconds
#include <csignal>        // For signal handling (Ctrl+C)

// Global flag to control the main loop
volatile bool g_running = true;

// Signal handler for Ctrl+C
void signalHandler(int signum) {
    if (signum == SIGINT) {
        g_running = false;
    }
}

int main() {
    // Register signal handler for graceful shutdown
    signal(SIGINT, signalHandler);

    OrderBook book;
    Dashboard dashboard;
    OrderGenerator generator(100.0); // Start with a mid-price of $100.00
    
    int loopCount = 0;

    std::cout << "Starting CLOB Engine... Press Ctrl+C to stop." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));

    while (g_running) {
        // 1. Generate a new random order
        Order newOrder = generator.generateOrder();

        // 2. Add the order to the book (triggers matching)
        book.addOrder(newOrder);

        // 3. Update the dashboard
        dashboard.display(book);

        // Add a small delay to make it "real-time" and viewable
        // In a real system, this loop would run as fast as possible
        // or be event-driven.
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        
        loopCount++;
    }

    // Cleanup: Clear the screen and show a shutdown message
    std::cout << "\033[2J\033[H" << std::flush;
    std::cout << "CLOB Engine shutting down." << std::endl;
    std::cout << "Processed " << loopCount << " orders." << std::endl;

    return 0;
}
