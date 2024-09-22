#pragma once

#include <HeadersBase.hpp>

#include <LogsManager.hpp>

class SystemProfiling : public LogsManager{
    public:
        SystemProfiling();
        ~SystemProfiling() = default;

        std::string getHostname();
        std::string getOS();
        double getRAM();

        std::string getCPUName();
        std::string getGPUName();

    private:
        char hostname[256], os[256];
        struct sysinfo si;
        FILE *fp;

        double RAM;

        std::string CPU_name, GPU_name;
};