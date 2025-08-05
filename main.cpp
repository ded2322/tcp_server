/*
****************************
*** Main cpp file, where start server 
****************************
*/
#define MAYBE_UNUSED __attribute__((unused))
#define COUNT_THREADS 5

#include <iostream>
#include <cstdio>
#include <string>
#include <thread>
#include <atomic>
#include <csignal>
#include <cstring>
#include <memory>
#include <vector>

#include "tcp_server.h"
// TODO write to file for client
// TODO include flags

std::atomic<bool> server_is_running {true};

static struct sigaction sa;

// Cmake ругается что не определено заранее
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

    std::vector<std::thread> threads(COUNT_THREADS);
    std::vector<int> sock_client(COUNT_THREADS);


    for (std::size_t thread {0}; thread < COUNT_THREADS; ++thread) {
        while (server_is_running) {
            try{

               std::unique_ptr<Connection> user_connection = server.acceptClient();
               
                threads.emplace_back( [&server] (std::unique_ptr<Connection> connection) mutable {
                    server.handlerClient(connection);
                    std::cout << "Connection with client close\n";
                }, std::move(user_connection));
                
            } catch (const char* exep) {
                std::cerr << "User_connection: " << exep << "\n";
            }
        }
    }
    
    for (auto& thread: threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }

    server.~TcpServer();
    std::cout << "Server turn off\n";
    exit(0);
}