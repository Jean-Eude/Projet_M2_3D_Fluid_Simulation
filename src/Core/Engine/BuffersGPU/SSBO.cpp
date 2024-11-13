#include <SSBO.hpp>

SSBO::~SSBO() {
    unbindSSBO();
}

void SSBO::bindSSBO() {
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->SSBO_id);
}

void SSBO::unbindSSBO() {
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

std::string& SSBO::getName() {
    return m_name;
}

void SSBO::setName(const std::string& name) {
    m_name = name;
}

GLuint SSBO::getSSBO_ID() {
    return this->SSBO_id;
}

GLenum SSBO::getSSBO_Usage() {
    return this->usage;
}

GLenum SSBO::getSSBO_Mode() {
    return this->mode;
}


size_t SSBO::getDataSize() {
    return this->dataSize;
}
