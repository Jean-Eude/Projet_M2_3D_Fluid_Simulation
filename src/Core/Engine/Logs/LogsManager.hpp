#pragma once

#include <ConsoleLogger.hpp>
#include <FileLogger.hpp>

#include <ThreadManager.hpp>

class LogsManager : public ThreadManager {
    protected:
        LogsManager() : fileLogger("log.txt"), sleepLogsDuration(1) {}
        virtual ~LogsManager() = default;

        ConsoleLogger consoleLogger;
        FileLogger fileLogger;
        std::vector<std::pair<std::string, LogLevel>> logQueue;
        std::mutex logQueueMutex;
        int64_t sleepLogsDuration;

    public:
        virtual void enqueueConsoleLogs(std::string msg, LogLevel log) {
            {
                std::lock_guard<std::mutex> lock(logQueueMutex);
                logQueue.emplace_back(msg, log);
            }

            pool.enqueue([this, msg, log]() {
                //std::cout << "Thread ID (Info): " << std::this_thread::get_id() << std::endl;
                consoleLogger.log(msg, log, __FILE__, __LINE__);
            });

            std::this_thread::sleep_for(std::chrono::seconds(sleepLogsDuration));
        }

        virtual void enqueueConsoleLogsInit(std::string msg, LogLevel log) {
            {
                std::lock_guard<std::mutex> lock(logQueueMutex);
                logQueue.emplace_back(msg, log);
            }

            pool.enqueue([this, msg, log]() {
                //std::cout << "Thread ID (Info): " << std::this_thread::get_id() << std::endl;
                consoleLogger.log(msg, log, __FILE__, __LINE__);
            });
        }

        virtual void enqueueFileLogs() {
            std::lock_guard<std::mutex> lock(logQueueMutex);

            for (const auto& entry : logQueue) {
                const auto& [message, logLevel] = entry;

                pool.enqueue([this, message, logLevel]() {
                    fileLogger.log(message.c_str(), logLevel, __FILE__, __LINE__);
                });
            }

            logQueue.clear();

            std::this_thread::sleep_for(std::chrono::seconds(sleepLogsDuration));
        }
};