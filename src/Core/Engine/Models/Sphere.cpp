#include <Sphere.hpp>

const bool Sphere::registered = ModelManager::registerModel<Sphere, int>("Sphere");

Sphere::Sphere(int gridSize) : gridSize(gridSize) {}

void Sphere::InitVerticesAndIndices() {
    vertices.clear();
    indices.clear();

    // Génération des sommets
    for (unsigned int y = 0; y <= this->gridSize; ++y) {
        for (unsigned int x = 0; x <= this->gridSize; ++x) {
            // Calcul des segments UV
            float xSegment = static_cast<float>(x) / static_cast<float>(this->gridSize);
            float ySegment = static_cast<float>(y) / static_cast<float>(this->gridSize);

            // Calcul des coordonnées de la sphère
            float xPos = std::cos(xSegment * 2.0f * M_PI) * std::sin(ySegment * M_PI);
            float yPos = std::cos(ySegment * M_PI);
            float zPos = std::sin(xSegment * 2.0f * M_PI) * std::sin(ySegment * M_PI);

            // Ajouter la position
            vertices.push_back(xPos);
            vertices.push_back(yPos);
            vertices.push_back(zPos);

            // Ajouter les coordonnées UV
            vertices.push_back(xSegment);
            vertices.push_back(ySegment);

            // Ajouter la normale (identique à la position pour une sphère centrée à l'origine)
            glm::vec3 normal = glm::normalize(glm::vec3(xPos, yPos, zPos));
            vertices.push_back(normal.x);
            vertices.push_back(normal.y);
            vertices.push_back(normal.z);
        }
    }

    // Génération des indices pour les triangles
    for (unsigned int y = 0; y < this->gridSize; ++y) {
        for (unsigned int x = 0; x <= this->gridSize; ++x) {
            // Indices des sommets
            unsigned int current = y * (this->gridSize + 1) + x;
            unsigned int next = (y + 1) * (this->gridSize + 1) + x;

            // Triangle 1
            indices.push_back(current);
            indices.push_back(next);
            indices.push_back(current + 1);

            // Triangle 2
            indices.push_back(current + 1);
            indices.push_back(next);
            indices.push_back(next + 1);
        }
    }
}

void Sphere::bindBuffers() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Attribut de position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Attribut de coordonnées de texture
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Attribut de normale
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Sphere::Update() {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
