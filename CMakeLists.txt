cmake_minimum_required(VERSION 3.10)

project(tcp_server)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -Wpedantic -flto -O2 -fdiagnostics-color=always -ggdb3")

add_executable(
    tcp_server
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tcp_server.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tcp_client.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/utils_tcp.cpp
)
