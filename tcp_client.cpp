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
        int sock = -1;
        struct sockaddr_in server_addr;
    public:
        TcpClient (int port): port(port), sock(-1) {
            server_addr.sin_family = AF_INET;
            server_addr.sin_port = htons(port);
            inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);
        }

        ~TcpClient() {
            close(sock);
            sock =-1;
        }

        void connectServer() {
            sock = socket(AF_INET, SOCK_STREAM, 0);
            if (sock < 0) strerror(errno); 

            if( connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) 
                throw sprintf((char*)"connect: %s (%d)", strerror(errno), errno); 
        }

        void sendMessage(const std::string&& message) {
            // TODO in future do loop, which can accept long message
            
            int bytes = send(sock, message.c_str(), message.length(), 0);

            if (bytes  < 0) strerror(errno); 
        }

        std::string readMessage() {
            // TODO create loop for reading long message
            // TODO  corrent EINTR and EAGAIN error
            char buffer[1024] = {0};

            int bites_received = read(sock, buffer, sizeof(buffer));
            
            if (bites_received < 0) strerror(errno);  

            return std::string(buffer, bites_received);
        }
};
