#include <Models.hpp>
#include <ModelManager.hpp>

Models::Models() : EBO(0), VBO(0), VAO(0) {}

Models::~Models() {
    Clear();
}

std::vector<float> Models::getVertices() {
    return vertices;
}

std::vector<unsigned int> Models::getIndices() {
    return indices;
}

int Models::getNbVerts() {
    return static_cast<int>(vertices.size());
}

int Models::getNbIndices() {
    return static_cast<int>(indices.size());
}

void Models::Init() {
    InitVerticesAndIndices();
    bindBuffers();
}

void Models::Clear() {
    vertices.clear();
    indices.clear();
    if (VAO) {
        glDeleteVertexArrays(1, &VAO);
        VAO = 0;
    }
    if (VBO) {
        glDeleteBuffers(1, &VBO);
        VBO = 0;
    }
    if (EBO) {
        glDeleteBuffers(1, &EBO);
        EBO = 0;
    }
}