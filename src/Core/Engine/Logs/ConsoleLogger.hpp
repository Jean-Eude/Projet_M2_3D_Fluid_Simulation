#pragma once

// Logs affichés dans la console

#include <Logger.hpp>

class ConsoleLogger : public Logger {
    public:
        virtual void log(const std::string &message, LogLevel level, const char *file, int line) override;
};