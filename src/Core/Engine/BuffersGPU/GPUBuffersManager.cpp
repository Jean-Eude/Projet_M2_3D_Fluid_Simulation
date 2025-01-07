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

void GPUBuffersManager::enqueueFBO(const std::string& name, int width, int height, FBOType type) {
    if (fboQueue.find(name) != fboQueue.end()) {
        std::cerr << "FBO named '" << name << "' already exists." << std::endl;
        return;
    }

    FBO fbo;
    fbo.createFBO(width, height, type);

    fboQueue[name] = fbo;
    fboTypes[name] = type;
}

void GPUBuffersManager::dequeueFBO(const std::string& name) {
    auto it = fboQueue.find(name);
    if (it != fboQueue.end()) {
        FBO& fbo = it->second;

        GLuint texID = fbo.getTexID();
        if (texID != 0) {
            glDeleteTextures(1, &texID);
        }

        GLuint rboID = fbo.getRBO();
        if (rboID != 0) {
            glDeleteRenderbuffers(1, &rboID);
        }

        GLuint fboID = fbo.getFBO();
        if (fboID != 0) {
            glDeleteFramebuffers(1, &fboID);
        }

        fboQueue.erase(it);
        fboTypes.erase(name);
    } else {
        std::cerr << "WARNING::GPUBuffersManager:: FBO named '" << name << "' not found." << std::endl;
    }
}

void GPUBuffersManager::dequeueAllFBO() {
    for (auto& pair : fboQueue) {
        FBO& fbo = pair.second;

        GLuint texID = fbo.getTexID();
        if (texID != 0) {
            glDeleteTextures(1, &texID);
        }

        GLuint rboID = fbo.getRBO();
        if (rboID != 0) {
            glDeleteRenderbuffers(1, &rboID);
        }

        GLuint fboID = fbo.getFBO();
        if (fboID != 0) {
            glDeleteFramebuffers(1, &fboID);
        }
    }

    fboQueue.clear();
    fboTypes.clear();
}

void GPUBuffersManager::rescaleFBO(const std::string& name, int width, int height) {
    auto it = fboQueue.find(name);
    if (it != fboQueue.end()) {
        FBO& fbo = it->second;
        fbo.rescaleFBO(width, height);
    } else {
        std::cerr << "WARNING::GPUBuffersManager:: FBO named '" << name << "' not found. Cannot rescale." << std::endl;
    }
}

void GPUBuffersManager::bindFBO(const std::string& name) {
    auto it = fboQueue.find(name);
    if (it != fboQueue.end()) {
        it->second.bindFBO();
        it->second.bindRBO();
    } else {
        std::cerr << "FBO named '" << name << "' not found." << std::endl;
    }
}

void GPUBuffersManager::unbindFBO(const std::string& name) {
    auto it = fboQueue.find(name);
    if (it != fboQueue.end()) {
        it->second.unbindFBO();
        it->second.unbindRBO();
    } else {
        std::cerr << "FBO named '" << name << "' not found." << std::endl;
    }
}

GLuint GPUBuffersManager::getFBO_IDByName(const std::string& name) {
    auto it = fboQueue.find(name);
    if (it != fboQueue.end()) {
        return it->second.getFBO();
    }
    return 0;
}

GLuint GPUBuffersManager::getFBO_Tex_IDByName(const std::string& name) {
    auto it = fboQueue.find(name);
    if (it != fboQueue.end()) {
        return it->second.getTexID();
    }
    return 0;
}

std::pair<int, int> GPUBuffersManager::getFBODimensionsByName(const std::string& name) {
    auto it = fboQueue.find(name);
    if (it != fboQueue.end()) {
        return { it->second.getFBOWidth(), it->second.getFBOHeight() };
    }
    return { 0, 0 };
}

FBOType GPUBuffersManager::getFBOTypeByName(const std::string& name) {
    auto it = fboTypes.find(name);
    if (it != fboTypes.end()) {
        return it->second;
    }
    throw std::runtime_error("FBO type not found for '" + name + "'.");
}