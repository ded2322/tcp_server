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

class TcpClient{
    private:
        int port;
        int client_sock = -1;
        struct sockaddr_in server_addr;
    public:
        TcpClient (int port_value): port(port_value), client_sock(-1) {
            server_addr.sin_family = AF_INET;
            server_addr.sin_port = htons(port_value);
            inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);
        }

        ~TcpClient() {
            close(client_sock);
            client_sock =-1;
        }

        int connectServer() {
            client_sock = socket(AF_INET, SOCK_STREAM, 0);
            if (client_sock < 0) strerror(errno); 

            if( connect(client_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) 
                throw strerror(errno);
                // throw sprintf((char*)"connect: %s (%d)", strerror(errno), errno);
            return client_sock;
        }
};
