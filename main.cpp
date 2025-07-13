#include <iostream>
#include <cstdio>
#include <string>
#include <pthread.h>

#include "tcp_server.h"
// TODO write file for client
// TODO include client for flags

int main() {
    TcpServer server(8080);

    if (!server.startTcpServer()) return -1;
     
    while (true) {
        server.acceptClient();
        server.handlerClient();
    }
}