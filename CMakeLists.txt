cmake_minimum_required(VERSION 3.10)

project(tcp_server)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -flto -O2 -D_FORTIFY_SOURCE=2 \
    -Wall -Wextra -Wunused-result \
    -Wformat=2 -Wstrict-prototypes \
    -Wmissing-declarations -Wswitch-unreachable \
    -Wshadow -fstack-protector-strong \
    -fdiagnostics-color=always -ggdb3")

add_executable(
    tcp_server
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tcp_server.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tcp_client.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tcp_conn.cpp
)
