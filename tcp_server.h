#include <string>
#include <netinet/in.h>
#include <optional>

#pragma once

class TcpServer {
    private:
        int port;
        int listen_sock;
        int client;
        struct sockaddr_in address;
    public:
        TcpServer (int port);
        ~TcpServer();

        void handlerClient();
        void closeUserConnection();
        void closeTcpServer();
        bool startTcpServer();
        bool acceptClient();
        bool sendMessage(const std::string&& message);
        std::optional<std::string> readMessage();
};