/*
****************************
*** File for user process
****************************
*/
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <optional>
#include <cstring>
#include <iostream>

#include "utils_tcp.h"

UserConnection::UserConnection(int client_sock): client_sock(client_sock) {}

void UserConnection::closeUserConnection() {
    close(client_sock);
    std::cout << "Connection with client close\n"; 
}

std::optional<std::string> UserConnection::readMessage() {
    // TODO create loop for reading long message
    // TODO  corrent EINTR and EAGAIN error
    if(client_sock < 0) return "";

    char buffer[1024] = {0};
    int bytes = read(client_sock, buffer, 1024);

    if( bytes <= 0 ) {
        std::cerr << "Client close connection\n";
        return std::nullopt;
    }

    std::string message(buffer);
    if (!message.empty() && message.back() == '\r') message.pop_back();  
    if (!message.empty() && message.back() == '\n') message.pop_back();

    return message;
} 

bool UserConnection::sendMessage(const std::string&& message) {
    if(client_sock < 0) return false;

    std::size_t bite_send = send(client_sock, message.c_str(), message.length(), MSG_NOSIGNAL);

    if(bite_send < 0) {
        std::cerr << "Failed send message\n";
        return false;
    }
    
    return bite_send == static_cast<std::size_t>(message.length());
}
