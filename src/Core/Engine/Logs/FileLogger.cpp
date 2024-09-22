#include <FileLogger.hpp>

FileLogger::FileLogger() {
    m_logFile.open("log.txt", std::ios::out | std::ios::app);
    if (!m_logFile.is_open())
    {
        std::cerr << "Failed to open log file: log.txt" << std::endl;
    }
}

FileLogger::FileLogger(const std::string &path) {
    m_logFile.open(path, std::ios::out | std::ios::app);
    if (!m_logFile.is_open())
    {
        std::cerr << "Failed to open log file: " << path << std::endl;
    }
}

FileLogger::~FileLogger()
{
    if (m_logFile.is_open())
    {
        m_logFile.close();
    }
}

void FileLogger::log(const std::string &message, LogLevel level, const char *file, int line) {
    if (!m_logFile.is_open())
    {
        std::cerr << "Attempt to log to an unopened file." << std::endl;
        return;
    }

    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm now_tm = *std::localtime(&now_time);

    std::string levelStr = logLevelToString(level);
    m_logFile << "[" << std::put_time(&now_tm, "%Y-%m-%d %H:%M:%S") << "] " << "[" << levelStr << "] ";

    if (file != nullptr)
    {
        m_logFile << "[" << file << ":" << line << "] ";
    }

    m_logFile << message << std::endl;
}