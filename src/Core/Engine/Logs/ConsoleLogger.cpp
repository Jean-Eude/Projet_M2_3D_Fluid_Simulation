#include <ConsoleLogger.hpp>

void ConsoleLogger::log(const std::string &message, LogLevel level, const char *file, int line) {
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm now_tm = *std::localtime(&now_time);

    std::string colorCode;
    switch (level) {
        case LogLevel::DEBUG:
            colorCode = "\033[36m"; // Cyan
            break;
        case LogLevel::INFO:
            colorCode = "\033[0m";  // white
            break;
        case LogLevel::OK:
            colorCode = "\033[32m"; // green
            break;
        case LogLevel::WARNING:
            colorCode = "\033[33m"; // Yellow
            break;
        case LogLevel::ERROR:
            colorCode = "\033[31m"; // Red
            break;
        case LogLevel::CRITICAL:
            colorCode = "\033[35m"; // Magenta
            break;
        default:
            colorCode = "\033[0m"; // Reset
            break;
    }

    std::string levelStr = logLevelToString(level);
    std::cout << colorCode << "[" << std::put_time(&now_tm, "%Y-%m-%d %H:%M:%S") << "] " << "[" << levelStr << "]";

    if (file != nullptr) {
        std::cout << " [" << file << ":" << line << "]";
    }

    std::cout << " " << message << "\033[0m" << std::endl;
}