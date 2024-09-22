#include <SystemProfiling.hpp>

SystemProfiling::SystemProfiling() : fp(nullptr) {
    // Nom de la machine
    gethostname(this->hostname, sizeof(&hostname));
    LogsManager::enqueueConsoleLogsInit(std::string("Hostname :") + std::string(this->hostname), LogLevel::DEBUG);

    // Nom de l'OS
    this->fp = popen("uname -sr", "r");
    fgets(os, sizeof(os), fp);
    pclose(fp); 
    LogsManager::enqueueConsoleLogsInit(std::string("OS :") + std::string(os), LogLevel::DEBUG);

    // Nom du CPU    
    std::ifstream cpuinfo("/proc/cpuinfo");
    if (!cpuinfo) {
        LogsManager::enqueueConsoleLogsInit(std::string("CPU :") + "Failed to open /proc/cpuinfo", LogLevel::ERROR);
        return;
    }

    std::string line;
    while (std::getline(cpuinfo, line)) {
        if (line.length() >= 10 && line.substr(0, 10) == "model name") {
            std::size_t colon_pos = line.find(':');
            if (colon_pos != std::string::npos) {
                this->CPU_name = line.substr(colon_pos + 2);
            }
        }
    }
    LogsManager::enqueueConsoleLogsInit(std::string("CPU :") + CPU_name, LogLevel::DEBUG);

    // Nom du GPU
    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* vendor = glGetString(GL_VENDOR);

    if (renderer && vendor) {
        this->GPU_name = std::string(reinterpret_cast<const char*>(vendor)) + " " + std::string(reinterpret_cast<const char*>(renderer));
        LogsManager::enqueueConsoleLogsInit(std::string("GPU :") + this->GPU_name, LogLevel::DEBUG);
    } else {
        LogsManager::enqueueConsoleLogsInit("Could not retrieve GPU information from OpenGL", LogLevel::ERROR);
        this->GPU_name = "Unknown GPU";
        LogsManager::enqueueConsoleLogsInit(this->GPU_name, LogLevel::ERROR);
    }
    
    // Nombre total de RAM
    sysinfo(&this->si);
    this->RAM = this->si.totalram * this->si.mem_unit / (1024 * 1024 * 1024); // Conversion en Go*/ 
    std::string ramStr = std::to_string(RAM);
    LogsManager::enqueueConsoleLogsInit(std::string("RAM :") + ramStr, LogLevel::DEBUG);
}

std::string SystemProfiling::getHostname() {
    return this->hostname;
}

std::string SystemProfiling::getOS() {
    return this->os;
}

double SystemProfiling::getRAM() {
    return this->RAM;
}

std::string SystemProfiling::getCPUName() {
    return this->CPU_name;
}

std::string SystemProfiling::getGPUName() {
    return this->GPU_name;
}