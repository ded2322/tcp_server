/*
****************************
*** Main cpp file, where start server 
****************************
*/
#define MAYBE_UNUSED __attribute__((unused))

#include <iostream>
#include <cstdio>
#include <string>
#include <pthread.h>
#include <atomic>
#include <csignal>
#include <cstring>

#include "tcp_server.h"
// TODO write to file for client
// TODO include flags
// TODO add therads
std::atomic<bool> server_is_running {true};

static struct sigaction sa;

void terminate([[maybe_unused]] int signum) {
    server_is_running = false;
}


int main() {
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
    
    while (server_is_running) {
        try{
            Connection user_connection { server.acceptClient() };
            server.handlerClient( user_connection );
            std::cout << "Connection with client close\n";
        } catch (const char* exep) {
            std::cerr << "User_connection: " << exep << "\n";
        }
    }

    server.~TcpServer();
    std::cout << "Server turn off\n";
    exit(0);
}