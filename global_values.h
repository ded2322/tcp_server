#pragma once

#include <atomic>

#define MAYBE_UNUSED __attribute__((unused))
#define COUNT_THREADS 4
#define maxTcpConnection 5
 
inline std::atomic<bool> server_is_running {true};
