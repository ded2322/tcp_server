/*
****************************
*** Main cpp file, where start server 
****************************
*/
#include <iostream>
#include <cstdio>
#include <string>
#include <pthread.h>

#include "tcp_server.h"

// TODO write to file for client
// TODO include flags
// TODO add therads

int main() {
    TcpServer server(8080);

    try {
        server.startTcpServer();
    } catch (const char* exep) {
        std::cerr << "startTcpServer: " << exep << "\n";
        return -1;
    }
     
    while (true) {
        try{
            UserConnection user_connection { server.acceptClient() };
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