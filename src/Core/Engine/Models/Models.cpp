#include <Models.hpp>

Models::Models() {

}

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
    return vertices.size();
}

int Models::getNbIndices() {
    return indices.size();
}

void Models::Init() {

}

void Models::Update() {
    
}

void Models::Clear() {
    vertices.clear();
    indices.clear();
}