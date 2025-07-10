cmake_minimum_required(VERSION 3.10)

project(tcp_server)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -Wpedantic -flto -O2 -fdiagnostics-color=always -ggdb3")

add_executable(
    tcp_server
    main.cpp
    tcp_server.cpp
    tcp_client.cpp
)

set(HEADERS
    tcp_server.h`
)