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
    client_sock = -1;
}

std::optional<std::string> UserConnection::readMessage() {
    // TODO create loop for reading long message
    // TODO  corrent EINTR and EAGAIN error
    if ( client_sock < 0 ) throw "Failed read message: no client";

    char buffer[1024] = {0};
    ssize_t bytes = read(client_sock, buffer, 1024);

    if ( bytes == 0 ) return std::nullopt;
    if ( bytes < 0 ) strerror(errno);

    return std::string(buffer);
} 

void UserConnection::sendMessage(const std::string&& message) {
    // TODO in future do loop, which can accept long message
    if ( client_sock < 0 ) throw "Failed send message: no client";

    ssize_t byte_send = send(client_sock, message.c_str(), message.length(), MSG_NOSIGNAL);

    if (byte_send < 0) strerror(errno);
}
