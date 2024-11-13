#include <GPUBuffersManager.hpp>

FBO GPUBuffersManager::m_fbo;

void GPUBuffersManager::dequeueSSBO(const std::string& name) {
    auto it = buffersQueue.find(name);

    if (it != buffersQueue.end()) {
        buffersQueue.erase(it);
        bufferUnits.erase(name);
    } else {
        std::cerr << "Erreur : le SSBO nommé '" << name << "' n'existe pas." << std::endl;
    }
}

void GPUBuffersManager::dequeueAllSSBO() {
    buffersQueue.clear();
    bufferUnits.clear();
}

unsigned int GPUBuffersManager::getSSBO_IDByName(const std::string& name) {
    auto it = buffersQueue.find(name);
    if (it != buffersQueue.end()) {
        return it->second->getSSBO_ID();
    }
    return 0;
}

GLenum GPUBuffersManager::getSSBO_UsageByName(const std::string& name) {
    auto it = buffersQueue.find(name);
    if (it != buffersQueue.end()) {
        return it->second->getSSBO_Usage();
    }
    return 0;
}

GLenum GPUBuffersManager::getSSBO_ModeByName(const std::string& name) {
    auto it = buffersQueue.find(name);
    if (it != buffersQueue.end()) {
        return it->second->getSSBO_Mode();
    }
    return 0;
}

unsigned int GPUBuffersManager::getBufferUnit(const std::string& name) {
    auto it = bufferUnits.find(name);
    if (it != bufferUnits.end()) {
        return it->second;
    }
    return -1;    
} 