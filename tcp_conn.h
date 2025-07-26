#pragma once

#include <netinet/in.h>
#include <sys/socket.h>
#include <string>
#include <optional>

class Connection {
    private:
        int sock = -1;
        struct sockaddr_in client_addr;

    public:
        Connection(int client_sock);
        void closeConnection();
        void sendMessage(const std::string&& message);
        std::optional<std::string> readMessage();
};