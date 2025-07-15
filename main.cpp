/*
****************************
*** Main cpp file, where start server 
****************************
*/
#include <iostream>
#include <cstdio>
#include <string>
#include <pthread.h>

#include "tcp_server.h"
// #include "utils_tcp.h"
// TODO write file for client
// TODO include client for flags

int main() {
    TcpServer server(8080);

    if (!server.startTcpServer()) return -1;
     
    while (true) {
        UserConnection user_connection {server.acceptClient()};
        server.handlerClient(user_connection);
    }
}