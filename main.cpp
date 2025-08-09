/*
****************************
*** Main cpp file, where start server 
****************************
*/
#include <iostream>
#include <csignal>
#include <cstring>

#include "thead_pull.h"

static struct sigaction sa;
static void terminate([[maybe_unused]] int signum) {
    server_is_running = false;
}

int main() {
    // For Unix-signal
    memset(&sa, 0, sizeof(struct sigaction));
    sa.sa_handler = terminate;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGTERM, &sa, 0);
    sigaction(SIGINT, &sa, 0);

    TcpServer server(8080);
    
    try {
        server.startTcpServer();
    } catch (const char* exep) {
        std::cerr << "startTcpServer: " << exep << "\n";
        return -1;
    }

    ThreadPool thread_pool {server};
    try {
        thread_pool.start_worker(COUNT_THREADS);
    } catch (const char* exep) {
        std::cerr << "thread_pool: " << exep << "\n";
        thread_pool.stop_worker();
        return -1;
    }

    server.~TcpServer();
    std::cout << "Server turn off\n";
    exit(0);
}