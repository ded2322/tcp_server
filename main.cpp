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
#include <vector>

#include "tcp_server.h"
// TODO write to file for client
// TODO include flags

std::atomic<bool> server_is_running {true};

static struct sigaction sa;

void terminate([[maybe_unused]] int signum) {
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

    while (server_is_running) {
        try{

            for (std::size_t thread {0}; thread < COUNT_THREADS; ++thread) {
                Connection user_connection { server.acceptClient() };
                // server.handlerClient( user_connection );

                threads.emplace_back( [&server, conn = std::move(user_connection)]() mutable {
                    server.handlerClient(conn);
                });
            }
            for (auto& thread: threads) {
                thread.join();
                thread.detach();
            }
            std::cout << "Connection with client close\n";
        } catch (const char* exep) {
            std::cerr << "User_connection: " << exep << "\n";
        }
    }

    server.~TcpServer();
    std::cout << "Server turn off\n";
    exit(0);
}