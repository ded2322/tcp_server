/*
* Cpp file responsible for tcp client 
*/
#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

class TcpClient{
    private:
        int port;
        int sock;
        struct sockaddr_in server_addr;
    public:
        TcpClient (int port): port(port) {
            server_addr.sin_family = AF_INET;
            server_addr.sin_port = htons(port);
            inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);
        }
        ~TcpClient() {
            close(sock);
        }
        void connectServer() {
            sock = socket(AF_INET, SOCK_STREAM, 0);
            connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
        }
        bool sendMessage(const std::string&& message) {
            int bite_send = send(sock, message.c_str(), message.length(), 0);

            if (bite_send < 0) {
                std::cerr << "Failed send messag\n";
                return false;
            }
            return true;
        }
        std::string acceptMessage() {
            char buffer[1024] = {0};

            int bites_received = recv(sock, buffer, sizeof(buffer), 0);
            
            if (bites_received < 0) {
                std::cerr << "Failed read message\n";
                return "";
            }

            return std::string(buffer, bites_received);
        }
};

// int main() {
//     TcpClient client(8080);

//     client.connectServer();

//     while (true) {
//         std::cout << client.acceptMessage();
//         if (client.sendMessage("Test message")) std::cout << "Succeded send message\n";
//         // sleep(60);
//     }
// }
