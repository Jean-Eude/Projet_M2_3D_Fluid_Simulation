#include <Skybox.hpp>

const bool Skybox::registered = ModelManager::registerModel<Skybox, int>("Skybox");

Skybox::Skybox(int skyboxSize) : skyboxSize(skyboxSize) {}

void Skybox::InitVerticesAndIndices() {
    vertices.clear();
    indices.clear();

    vertices = {
        -this->skyboxSize,  this->skyboxSize, -this->skyboxSize, // 0 : arrière haut gauche
        -this->skyboxSize, -this->skyboxSize, -this->skyboxSize, // 1 : arrière bas gauche
         this->skyboxSize, -this->skyboxSize, -this->skyboxSize, // 2 : arrière bas droite
         this->skyboxSize,  this->skyboxSize, -this->skyboxSize, // 3 : arrière haut droite

        -this->skyboxSize,  this->skyboxSize,  this->skyboxSize, // 4 : avant haut gauche
        -this->skyboxSize, -this->skyboxSize,  this->skyboxSize, // 5 : avant bas gauche
         this->skyboxSize, -this->skyboxSize,  this->skyboxSize, // 6 : avant bas droite
         this->skyboxSize,  this->skyboxSize,  this->skyboxSize  // 7 : avant haut droite
    };

    indices = {
        // Face arrière
        0, 1, 2, 2, 3, 0,
        // Face avant
        4, 5, 6, 6, 7, 4,
        // Face gauche
        0, 4, 5, 5, 1, 0,
        // Face droite
        3, 2, 6, 6, 7, 3,
        // Face dessous
        1, 5, 6, 6, 2, 1,
        // Face dessus
        0, 3, 7, 7, 4, 0
    };
}

void Skybox::bindBuffers() {
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

void Skybox::Update() {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}