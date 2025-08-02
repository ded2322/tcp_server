/*
****************************
*** File for start tcp server and user handler
****************************
*/
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <optional>
#include <cstring>
#include <errno.h>
#include <format>
#include <memory>

#include "tcp_server.h"

#define maxTcpConnection 5

TcpServer::TcpServer (int usr_port): port(usr_port), listen_sock(-1) {
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(usr_port);
}

TcpServer::~TcpServer() {
    close(listen_sock);
    listen_sock = -1;
}

void TcpServer::startTcpServer() {
    // Создание сокета
    listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if ( listen_sock < 0 ) throw strerror(errno); 

    // Разрешение для повторного использования адреса
    int opt = 1;
    if (setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) throw strerror(errno); 

    // Резервация порта
    if (bind(listen_sock, (struct sockaddr *)&address, sizeof(address)) < 0) throw strerror(errno); 

    // Начинаем слушать соединения
    if (listen(listen_sock, maxTcpConnection) < 0) throw strerror(errno);

    std::cout << "Server start\n";
}

int TcpServer::acceptClient() {
    socklen_t addrlen = sizeof(address);
    int client = accept(listen_sock, (struct sockaddr *)&address, (socklen_t *)&addrlen);

    if ( client <= 0 ) throw strerror(errno); 
    
    std::cout << "Accept user connection\n";
    return client;
}

void TcpServer::handlerClient(std::unique_ptr<Connection>& user_connection) {
    user_connection->sendMessage(std::move("Hello :)\n") );

    while (true) {
        user_connection->sendMessage("Input: ");
        try {
            
            auto user_message { user_connection->readMessage() };

            if (!user_message.has_value()) { user_connection->closeConnection(); return; }

            user_connection->sendMessage("Your input: " + user_message.value() + "\n");

        } catch (const char* exep) {
            std::cerr << "handlerClient: " << exep;
        }
    }
}
