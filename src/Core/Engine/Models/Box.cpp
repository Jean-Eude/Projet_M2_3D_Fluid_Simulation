#include <Box.hpp>

const bool Box::registered = ModelManager::registerModel<Box, float>("Box");

Box::Box(float boxSize) : boxSize(boxSize) {}

void Box::InitVerticesAndIndices() {
    vertices.clear();
    indices.clear();

    vertices.push_back(-boxSize); vertices.push_back(-boxSize); vertices.push_back(-boxSize); // 0
    vertices.push_back(boxSize);  vertices.push_back(-boxSize); vertices.push_back(-boxSize); // 1
    vertices.push_back(boxSize);  vertices.push_back(boxSize);  vertices.push_back(-boxSize); // 2
    vertices.push_back(-boxSize); vertices.push_back(boxSize);  vertices.push_back(-boxSize); // 3
    vertices.push_back(-boxSize); vertices.push_back(-boxSize); vertices.push_back(boxSize);  // 4
    vertices.push_back(boxSize);  vertices.push_back(-boxSize); vertices.push_back(boxSize);  // 5
    vertices.push_back(boxSize);  vertices.push_back(boxSize);  vertices.push_back(boxSize);  // 6
    vertices.push_back(-boxSize); vertices.push_back(boxSize);  vertices.push_back(boxSize);  // 7

    indices.push_back(0); indices.push_back(1);
    indices.push_back(1); indices.push_back(2);
    indices.push_back(2); indices.push_back(3);
    indices.push_back(3); indices.push_back(0);
    indices.push_back(4); indices.push_back(5);
    indices.push_back(5); indices.push_back(6);
    indices.push_back(6); indices.push_back(7);
    indices.push_back(7); indices.push_back(4);
    indices.push_back(0); indices.push_back(4);
    indices.push_back(1); indices.push_back(5);
    indices.push_back(2); indices.push_back(6);
    indices.push_back(3); indices.push_back(7);

    for (size_t i = 0; i < vertices.size(); i += 3) {
        glm::vec3 vertex(vertices[i], vertices[i + 1], vertices[i + 2]);
        minAABB = glm::min(minAABB, vertex);
        maxAABB = glm::max(maxAABB, vertex);
    }
}

void Box::bindBuffers() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Box::Update() {
    glBindVertexArray(VAO);
    glDrawElements(GL_LINES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

glm::vec3 Box::getBBmin() {
    return minAABB;
}

glm::vec3 Box::getBBmax() {
    return maxAABB;
}