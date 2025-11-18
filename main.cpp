#include "Order.h"
#include "dashboard.h"
#include "utility.h"
#include <iostream>
#include <thread>         
#include <chrono>        
#include <csignal>       


using namespace std;

// Global flag to control the main loop
volatile bool g_running = true;


void signalHandler(int signum) {
    if (signum == SIGINT) {
        g_running = false;
    }
}

int main() {
    signal(SIGINT, signalHandler);

    OrderBook book;
    Dashboard dashboard;
    OrderGenerator generator(100.0); // Start with a mid-price of $100.00
    
    int loopCount = 0;

    cout << "Starting CLOB Engine... Press Ctrl+C to stop." << endl; // very important
    this_thread::sleep_for(chrono::seconds(2)); 

    while (g_running) {
        
        Order newOrder = generator.generateOrder();

       
        book.addOrder(newOrder);

        dashboard.display(book);

       
        this_thread::sleep_for(chrono::milliseconds(200)); 
        
        loopCount++;
    }

    //Referenced from chatgpt:
    // Cleanup
    cout << "\033[2J\033[H" << flush;
    cout << "CLOB Engine shutting down." << endl; 
    cout << "Processed " << loopCount << " orders." << endl; 

    return 0;
}
