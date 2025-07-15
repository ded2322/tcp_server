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
// #include "utils_tcp.h"
// TODO write file for client
// TODO include client for flags

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
        } catch (const char* exep) {
            std::cerr << "User_connection: " << exep << "\n";
        }
    }
}