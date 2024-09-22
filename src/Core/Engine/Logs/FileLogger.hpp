#pragma once

// Logs enregistrés dans un fichier

#include <Logger.hpp>

class FileLogger : public Logger {
    private:
        std::ofstream m_logFile;

    public:
        FileLogger();
        FileLogger(const std::string &filePath);
        ~FileLogger();
        virtual void log(const std::string &message, LogLevel level, const char *file, int line) override;
};