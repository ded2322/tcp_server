/*
* Cpp file responsible for tcp client 
*/
#include <iostream>
#include <string>
#include <cstring>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <memory>

#include "tcp_conn.h"

class TcpClient{
    private:
        int port;
        struct sockaddr_in server_addr;
    public:
        TcpClient (int port_value): port(port_value) {
            server_addr.sin_family = AF_INET;
            server_addr.sin_port = htons(port_value);
            inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);
        }

        std::unique_ptr<Connection> connectServer() {
            int client_sock = socket(AF_INET, SOCK_STREAM, 0);
            if (client_sock < 0) throw strerror(errno); 

            if( connect(client_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) 
                throw strerror(errno);
            return std::make_unique<Connection>(client_sock);
        }
};
