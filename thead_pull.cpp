#include "thead_pull.h"


ThreadPool::ThreadPool (TcpServer& data_server) : server(data_server) {}

void ThreadPool::worker_loop() {
    std::unique_ptr<Connection> connection_user = nullptr;
    
    while (server_is_running) {
        
        queue_mutex.lock();
        if (!connection_queue.empty()) {
            connection_user = std::move(connection_queue.front());
            connection_queue.pop();
        }

        queue_mutex.unlock();
        
        if (connection_user && connection_user->isConnected()){
            server.handleClient(connection_user);
            connection_user->closeConnection();
            std::cout << "Connection with client close\n";
        } else {
            // Сделано как некая оптимизация чтобы не додосить мой одноядерный сервер
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }

    }
}


void ThreadPool::start_workers(int worker_count) {
    for (int count_thread {0}; count_thread < worker_count; ++count_thread) {
        threads.emplace_back(&ThreadPool::worker_loop, this);
    }
    while (server_is_running) {
        std::unique_ptr<Connection> connection_user = server.acceptClient();
        queue_mutex.lock();
        connection_queue.push(std::move(connection_user));
        queue_mutex.unlock();
        
    }
}

void ThreadPool::stop_worker() {
    for (auto& thread: threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}
