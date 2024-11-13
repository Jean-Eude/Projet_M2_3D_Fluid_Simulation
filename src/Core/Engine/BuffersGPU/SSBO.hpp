#pragma once

#include <HeadersBase.hpp>

class SSBO {
    public:
        SSBO() = default;
        ~SSBO();

        template <typename T>
        void createSSBO(GLuint unit, GLenum usage, const std::vector<T>& data);

        template <typename T>
        void linkSSBO_Datas(GLenum mode, std::vector<T>& outputData);

        void bindSSBO();
        void unbindSSBO();

        GLuint getSSBO_ID();
        GLenum getSSBO_Usage();
        GLenum getSSBO_Mode();
        size_t getDataSize();

        std::string& getName();
        void setName(const std::string& name);

    private:
        GLuint SSBO_id;
        GLenum usage; 
        GLenum mode;

        size_t dataSize;

        std::string m_name;
};

template <typename T>
void SSBO::createSSBO(GLuint unit, GLenum usage, const std::vector<T>& data) {
    this->usage = usage;

    this->dataSize = data.size() * sizeof(T);

    glGenBuffers(1, &this->SSBO_id);
    bindSSBO();
    glBufferData(GL_SHADER_STORAGE_BUFFER, this->dataSize, data.data(), usage);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, unit, this->SSBO_id);
}

template <typename T>
void SSBO::linkSSBO_Datas(GLenum mode, std::vector<T>& outputData) {
    this->mode = mode;

    bindSSBO();
    T* mappedData = static_cast<T*>(glMapBuffer(GL_SHADER_STORAGE_BUFFER, mode));

    if (mappedData) {
        outputData.resize(this->dataSize / sizeof(T));
        
        std::memcpy(outputData.data(), mappedData, this->dataSize);
        glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
    } else {
        std::cerr << "Erreur lors du mappage du SSBO." << std::endl;
    }

    unbindSSBO();
}