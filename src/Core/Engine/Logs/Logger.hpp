#pragma once

#include <HeadersBase.hpp>

enum class LogLevel {
    DEBUG,
    INFO,
    OK,
    WARNING,
    ERROR,
    CRITICAL
};

class Logger {
    protected:
        std::mutex logMutex;
        std::string logLevelToString(LogLevel level);

    public:
        virtual ~Logger() {}
        virtual void log(const std::string &message, LogLevel level, const char *file, int line) = 0;

        #define LOG(logger, level, message) (logger).log(message, level, __FILE__, __LINE__)
        #define LOG_DEBUG(logger, message) LOG(logger, LogLevel::DEBUG, message)
        #define LOG_OK(logger, message) LOG(logger, LogLevel::OK, message)
        #define LOG_INFO(logger, message) LOG(logger, LogLevel::INFO, message)
        #define LOG_WARNING(logger, message) LOG(logger, LogLevel::WARNING, message)
        #define LOG_ERROR(logger, message) LOG(logger, LogLevel::ERROR, message)
        #define LOG_CRITICAL(logger, message) LOG(logger, LogLevel::CRITICAL, message)
};
