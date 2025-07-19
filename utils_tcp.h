#pragma once

#include <netinet/in.h>
#include <sys/socket.h>
#include <string>
#include <optional>

class UserConnection {
    private:
        int client_sock;
        struct sockaddr_in client_addr;

    public:
        UserConnection(int client_sock);
        void closeUserConnection();
        void sendMessage(const std::string&& message);
        std::optional<std::string> readMessage();
};