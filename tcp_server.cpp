/*
****************************
*** File for start tcp server and user handler
****************************
*/
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <optional>
#include <cstring>
#include <iostream>

#include "tcp_server.h"

#define maxTcpConnection 5

TcpServer::TcpServer (int port): port(port), listen_sock(-1) {
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
}

TcpServer::~TcpServer() {
    closeTcpServer();
}

bool TcpServer::startTcpServer() {
    // Создание сокета
    listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock < 0) {
        std::cerr << "failed create socket\n";
        return false;
    }

    // Разрешение для повторного использования адреса
    int opt = 1;
    setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    // Резервация порта
    if (bind(listen_sock, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::cerr << "Failed create bind\n";
        return false;
    }
    // Начинаем слушать соединения
    if (listen(listen_sock, maxTcpConnection) < 0) {
        std::cerr << ("Failed start listen\n");
        return false;
    }
    std::cout << "Server start\n";
    return true;
}

void TcpServer::closeTcpServer() {
    close(listen_sock);
    std::cout << "listen_sock turn off\n";
    exit(0);
}

int TcpServer::acceptClient() {
    socklen_t addrlen = sizeof(address);
    int client = accept(listen_sock, (struct sockaddr *)&address, (socklen_t *)&addrlen);

    if(client <= 0) {
        std::cerr << "Failed accept connection\n";
        return -1;
    }

    std::cout << "Accept connection\n";
    return client;
}

void TcpServer::handlerClient(UserConnection user_connection) {
    std::string message { "Hello :)\n"};
    user_connection.sendMessage(std::move(message));

    while (true) {
        user_connection.sendMessage("Input: ");
        auto user_message {user_connection.readMessage()};
        if (!user_message.has_value()) return;

        user_connection.sendMessage("Your input: " + user_message.value() + "\n");
    }
}

// For test
// int main() {
//     TcpServer server(8080);
//     server.startTcpServer();

//     if (server.acceptClient()) {
//         server.sendMessage("Hello, glad to see u!\n");
//     }
    
//     while (true) {
//         std::string user_message { server.readMessage() };

//         std::cout << user_message << "\n";

//     }
// }