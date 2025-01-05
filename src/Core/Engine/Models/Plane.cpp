#include <Plane.hpp>

const bool Plane::registered = ModelManager::registerModel<Plane, float, int, bool>("Plane");

Plane::Plane(float size, int div, bool isBottom) : size(size), div(div), isBottom(isBottom) {}

void Plane::InitVerticesAndIndices() {
    vertices.clear();
    indices.clear();

    float cellSize = size / div;
    float halfSize = size / 2.0f;

    for (int z = 0; z <= div; ++z) {
        for (int x = 0; x <= div; ++x) {
            float xPos = x * cellSize - halfSize;
            float zPos = z * cellSize - halfSize;

            // Sommets
            vertices.push_back(xPos); 
            if(isBottom == false) {
                vertices.push_back(0.0f);  
            } else {
                vertices.push_back(halfSize);       
            }   
            vertices.push_back(zPos);

            // Coords de textures
            vertices.push_back((float)x / div); 
            vertices.push_back((float)z / div);

            // Normales
            vertices.push_back(0.0f);
            vertices.push_back(1.0f);
            vertices.push_back(0.0f);
        }
    }

    // Génération des indices
    for (int z = 0; z < div; ++z) {
        for (int x = 0; x < div; ++x) {
            int topLeft = z * (div + 1) + x;
            int topRight = topLeft + 1;
            int bottomLeft = (z + 1) * (div + 1) + x;
            int bottomRight = bottomLeft + 1;

            // 1er triangle
            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);

            // 2ème triangle
            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);
        }
    }
}

void Plane::bindBuffers() {
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

void Plane::Update() {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
