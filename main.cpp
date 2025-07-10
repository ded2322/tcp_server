#include <iostream>
#include <cstdio>
#include <string>
#include <pthread.h>

#include "tcp_server.h"
// TODO write file for client
// TODO include client for flags

int main() {

    std::string message = 
        "Hello :)\n" 
        "If you want leave server, input exit\n";

    TcpServer server(8080);

    if (!server.startTcpServer()) return -1;
    
    if (server.acceptClient()) {

        server.sendMessage(std::move(message));

        while (true) {
            server.sendMessage("Input: ");
            std::string user_message {server.readMessage()};
            
            if (user_message == "exit"){
                server.closeUserConnection();
                break;  
            } 

            std::cout << "User message: "<< user_message << "\n";
            server.sendMessage("Your input: " + user_message + "\n");
        }
    }
    server.closeTcpServer();
}