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

#include "tcp_conn.h"

Connection::Connection(int client_sock): sock(client_sock) {}

Connection::~Connection() {
    closeConnection();
}

void Connection::closeConnection() {
    close(sock);
    sock = -1;
}

std::optional<std::string> Connection::readMessage() {
    // TODO create loop for reading long message
    // TODO  corrent EINTR and EAGAIN error
    if ( sock < 0 ) throw "Failed read message: no client";

    char buffer[1024] = {0};
    ssize_t bytes = read(sock, buffer, 1024);

    if ( bytes == 0 ) return std::nullopt;
    if ( bytes < 0 ) throw strerror(errno);

    return std::string(buffer);
} 

void Connection::sendMessage(const std::string&& message) {
    // TODO in future do loop, which can accept long message
    if ( sock < 0 ) throw "Failed send message: no client";

    ssize_t byte_send = send(sock, message.c_str(), message.length(), MSG_NOSIGNAL);

    if (byte_send < 0) throw strerror(errno);
}
