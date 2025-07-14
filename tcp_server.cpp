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

TcpServer::TcpServer (int port): port(port), listen_sock(-1), client(-1) {
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
}

TcpServer::~TcpServer() {
    closeUserConnection();
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

void TcpServer::closeUserConnection() {
    close(client);
    std::cout << "Connection with client close\n"; 
}

void TcpServer::closeTcpServer() {
    close(listen_sock);
    std::cout << "listen_sock turn off\n";
    exit(0);
}

bool TcpServer::acceptClient() {
    socklen_t addrlen = sizeof(address);
    client = accept(listen_sock, (struct sockaddr *)&address, (socklen_t *)&addrlen);

    if(client < 0) {
        std::cerr << "Failed accept connection\n";
        return false;
    }

    std::cout << "Accept connection\n";
    return true;
}

std::optional<std::string> TcpServer::readMessage() {
    // TODO create loop for reading long message
    // TODO  corrent EINTR and EAGAIN error
    if(client < 0) return "";
    
    char buffer[1024] = {0};
    int bytes = read(client, buffer, 1024);

    if( bytes <= 0 ) {
        std::cerr << "Client close connection\n";
        return std::nullopt;
    }
    std::string message(buffer);
    if (!message.empty() && message.back() == '\r') message.pop_back();  
    if (!message.empty() && message.back() == '\n') message.pop_back();

    return message;
}
        
bool TcpServer::sendMessage(const std::string&& message) {
    if(client < 0) return false;

    std::size_t bite_send = send(client, message.c_str(), message.length(), MSG_NOSIGNAL);

    if(bite_send < 0) {
        std::cerr << "Failed send message\n";
        return false;
    }

    return bite_send == static_cast<std::size_t>(message.length());
}

void TcpServer::handlerClient() {
    std::string message { "Hello :)\n"};
    bool test = sendMessage(std::move(message));

    while (true) {
        sendMessage("Input: ");
        auto user_message {readMessage()};
        if (!user_message.has_value()) return;

        sendMessage("Your input: " + user_message.value() + "\n");
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