/*
****************************
*** File for start tcp server and user handler
****************************
*/
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <optional>
#include <cstring>
#include <errno.h>
#include <memory>

#include "tcp_server.h"

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

std::unique_ptr<Connection> TcpServer::acceptClient() {
    socklen_t addrlen = sizeof(address);
    int client = accept(listen_sock, (struct sockaddr *)&address, (socklen_t *)&addrlen);

    if ( client <= 0 ) throw strerror(errno); 
    
    std::cout << "Accept user connection\n";
    return std::make_unique<Connection>(client);
}

void TcpServer::handlerClient(std::unique_ptr<Connection>& connection_user) {
    connection_user->sendMessage(std::move("Hello :)\n") );

    while (true) {
        connection_user->sendMessage("Input: ");
        try {
            
            auto user_message { connection_user->readMessage() };

            if (!user_message.has_value()) { return; }

            connection_user->sendMessage("Your input: " + user_message.value() + "\n");

        } catch (const char* exep) {
            std::cerr << "handlerClient: " << exep;
        }
    }
}
