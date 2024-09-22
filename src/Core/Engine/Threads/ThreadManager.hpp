#pragma once

#include <ThreadPool.hpp>

class ThreadManager {
    protected:
        virtual ~ThreadManager() = default;

        ThreadManager() : pool(std::thread::hardware_concurrency()) {}

        ThreadPool pool;
};