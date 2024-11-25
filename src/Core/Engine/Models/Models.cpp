#include <Models.hpp

Models::Models() {

}

Models::~Models() {
    Clear();
}

Models::std::vector<unsigned int>getVertices() {
    return vertices;
}

Models::std::vector<unsigned int>getIndices() {
    return indices;
}

Models::int getNbVerts() {
    return vertices.size();
}

Models::int getNbIndices() {
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