#pragma once

#include <string>
#include <netinet/in.h>
#include <optional>

#include "tcp_conn.h"

class TcpServer {
    private:
        int port;
        int listen_sock = -1;
        struct sockaddr_in address;
    public:
        TcpServer (int port);
        ~TcpServer();

        void startTcpServer();
        int acceptClient();
        void handlerClient(Connection user_connection);
};