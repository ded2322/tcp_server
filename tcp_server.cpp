/*
* Cpp file responsible for tcp_server 
*/
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <iostream>

#include "tcp_server.h"

TcpServer::TcpServer (int port): port(port), server(-1), client(-1) {
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
}

TcpServer::~TcpServer() {
    closeTcpServer();
}

bool TcpServer::startTcpServer() {
    // Создание сокета
    server = socket(AF_INET, SOCK_STREAM, 0);
    if (server < 0) {
        std::cerr << "failed create socket\n";
        return false;
    }

    // Разрешение для повторного использования адреса
    int opt = 1;
    setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    // Резервация порта
    if (bind(server, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::cerr << "Failed create bind\n";
        return false;
    }
    // Начинаем слушать соединения
    if (listen(server, 5) < 0) {
        std::cerr << ("Failed start listen\n");
        return false;
    }
    std::cout << "Server start\n";
    return true;
}

void TcpServer::closeUserConnection() {
    close(client);
    std::cout << "Connection with client close\n"; 
}

void TcpServer::closeTcpServer() {
    closeUserConnection();
    close(server);
    std::cout << "Server turn off\n";
}

bool TcpServer::acceptClient() {
    socklen_t addrlen = sizeof(address);
    client = accept(server, (struct sockaddr *)&address, (socklen_t *)&addrlen);

    if(client < 0) {
        std::cerr << "Failed accept connection\n";
        return false;
    }

    std::cout << "Accept connection\n";
    return true;
}

std::string TcpServer::readMessage() {
    if(client < 0) return "";
    
    char buffer[1024] = {0};
    int bites_read = read(client, buffer, 1024);

    if( bites_read < 0 ) {
        std::cerr << "Failed read message\n";
        return "";
    }

    return std::string(buffer, bites_read-2);
}
        
bool TcpServer::sendMessage(const std::string&& message) {
    if(client < 0) return false;

    int bite_send = send(client, message.c_str(), message.length(), 0);

    if(bite_send < 0) {
        std::cerr << "Failed send message\n";
        return false;
    }
    return true;
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