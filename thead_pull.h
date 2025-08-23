#pragma once

#include <queue>
#include <memory>
#include <mutex>
#include <thread>

#include "tcp_conn.h"
#include "tcp_server.h"

// std::atomic<bool> server_is_running {true};

class ThreadPool {
    private:
        TcpServer server;

        std::queue<std::unique_ptr<Connection>> connection_queue;
        std::mutex queue_mutex;
        std::vector<std::thread> threads;

        void worker_loop();
    public:
        ThreadPool (TcpServer& data_server);
        void start_workers(int worker_count);
        void stop_worker();
};
