#include <GPUBuffersManager.hpp>

FBO GPUBuffersManager::m_fbo;

void GPUBuffersManager::dequeueSSBO(const std::string& name) {
    auto it = buffersQueue.find(name);

    if (it != buffersQueue.end()) {
        GLuint ssboID = it->second->getSSBO_ID();
        glDeleteBuffers(1, &ssboID);
        buffersQueue.erase(it);
        bufferUnits.erase(name);
    } else {
        std::cerr << "Erreur : le SSBO nommé '" << name << "' n'existe pas." << std::endl;
    }
}

void GPUBuffersManager::dequeueAllSSBO() {
    for (auto& pair : buffersQueue) {
        GLuint ssboID = pair.second->getSSBO_ID();
        glDeleteBuffers(1, &ssboID);
    }
    buffersQueue.clear();
    bufferUnits.clear();
}

void GPUBuffersManager::bindBufferBaseByName(const std::string& name) {
    auto it = buffersQueue.find(name);
    if (it != buffersQueue.end()) {
        auto& buffer = it->second;
        auto unitIt = bufferUnits.find(name);
        if (unitIt != bufferUnits.end()) {
            GLuint unit = unitIt->second;
            buffer->bindBufferBase(unit);
        } else {
            std::cerr << "Erreur : Unité du buffer '" << name << "' non trouvée." << std::endl;
        }
    } else {
        std::cerr << "Erreur : Buffer nommé '" << name << "' non trouvé." << std::endl;
    }
}

void GPUBuffersManager::bindBufferBaseAll() {
    for (const auto& pair : buffersQueue) {
        const std::string& name = pair.first;
        const auto& buffer = pair.second;

        auto unitIt = bufferUnits.find(name);
        if (unitIt != bufferUnits.end()) {
            GLuint unit = unitIt->second;
            buffer->bindBufferBase(unit); // Lie le buffer à son unité
        } else {
            std::cerr << "Erreur : Unité du buffer '" << name << "' non trouvée." << std::endl;
        }
    }
}

GLuint GPUBuffersManager::getSSBO_IDByName(const std::string& name) {
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