#pragma once

#include <string>
#include <netinet/in.h>
#include <optional>

#include "utils_tcp.h"

class TcpServer {
    private:
        int port;
        int listen_sock;
        struct sockaddr_in address;
    public:
        TcpServer (int port);
        ~TcpServer();

        void handlerClient(UserConnection user_connection);
        void closeTcpServer();
        int acceptClient();
        bool startTcpServer();
};