#pragma once

#include <FBO.hpp>
#include <SSBO.hpp>

class GPUBuffersManager {
    public:
        GPUBuffersManager() : nextUnit(0) {}
        ~GPUBuffersManager() = default;

        // FBO Management
        void enqueueFBO(const std::string& name, int width, int height, FBOType type);
        void dequeueFBO(const std::string& name);
        void rescaleFBO(const std::string& name, int width, int height);

        void dequeueAllFBO();
        void bindFBO(const std::string& name);
        void unbindFBO(const std::string& name);

        GLuint getFBO_IDByName(const std::string& name);
        GLuint getFBO_Tex_IDByName(const std::string& name);
        std::pair<int, int> getFBODimensionsByName(const std::string& name);
        FBOType getFBOTypeByName(const std::string& name);


        // SSBO Management
        template <typename T>
        void enqueueSSBO(const std::string& name, GLenum usage, const std::vector<T>& data);

        void dequeueSSBO(const std::string& name);
        void dequeueAllSSBO();

        template <typename T>
        void linkSSBOByName(const std::string& name, GLenum mode, std::vector<T>& outputData);

        template <typename T>
        void linkSSBOAll(GLenum mode, std::vector<T>& outputData);

        template <typename T>
        void UpdateSBOByName(const std::string& name, const std::vector<T>& newData);

        template <typename T>
        void UpdateSBOAll(const std::vector<T>& newData);

        void bindBufferBaseByName(const std::string& name);
        void bindBufferBaseAll();

        GLuint getSSBO_IDByName(const std::string& name);
        GLenum getSSBO_UsageByName(const std::string& name);
        GLenum getSSBO_ModeByName(const std::string& name);

        unsigned int getBufferUnit(const std::string& name);

    private:
        // SSBO Management
        std::map<std::string, std::shared_ptr<SSBO>> buffersQueue;
        std::map<std::string, unsigned int> bufferUnits;
        unsigned int nextUnit;

        // FBO Management
        std::map<std::string, FBO> fboQueue;
        std::map<std::string, FBOType> fboTypes;

    protected:
        static FBO m_fbo;
};

template <typename T>
void GPUBuffersManager::enqueueSSBO(const std::string& name, GLenum usage, const std::vector<T>& data) {
    if (buffersQueue.find(name) != buffersQueue.end()) {
        return;
    }

    auto buffer = std::make_shared<SSBO>();
    buffer->createSSBO(nextUnit, usage, data);
    buffer->setName(name);

    if (nextUnit < GL_MAX_COMBINED_SHADER_STORAGE_BLOCKS) {
        buffersQueue[name] = buffer;
        bufferUnits[name] = nextUnit;
        nextUnit++;
    } else {
        std::cerr << "Maximum buffers units reached. Unable to add buffer: " << name << std::endl;
    }    
}

template <typename T>
void GPUBuffersManager::linkSSBOByName(const std::string& name, GLenum mode, std::vector<T>& outputData) {
    auto it = buffersQueue.find(name);
    if (it != buffersQueue.end()) {
        auto& buffer = it->second;
        buffer->linkSSBO_Datas(mode, outputData);
    } 
}

template <typename T>
void GPUBuffersManager::linkSSBOAll(GLenum mode, std::vector<T>& outputData) {
    for (auto& pair : buffersQueue) {
        auto& buffer = pair.second;
        auto unit = bufferUnits[pair.first];
        buffer->linkSSBO_Datas(mode, outputData);
    }
}

template <typename T>
void GPUBuffersManager::UpdateSBOByName(const std::string& name, const std::vector<T>& newData) {
    auto it = buffersQueue.find(name);
    if (it != buffersQueue.end()) {
        auto& buffer = it->second;
        buffer->updateSSBO(newData);
    } 
}

template <typename T>
void GPUBuffersManager::UpdateSBOAll(const std::vector<T>& newData) {
    for (auto& pair : buffersQueue) {
        auto& buffer = pair.second;
        auto unit = bufferUnits[pair.first];
        buffer->updateSSBO(newData);
    }
}


