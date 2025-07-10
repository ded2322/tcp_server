#include <string>
#include <netinet/in.h>

#pragma once

class TcpServer {
    private:
        int port;
        int server;
        int client;
        struct sockaddr_in address;
    public:
        TcpServer (int port);
        ~TcpServer();

        bool startTcpServer();
        void closeUserConnection();
        void closeTcpServer();
        bool acceptClient();
        std::string readMessage();
        bool sendMessage(const std::string&& message);
        
};