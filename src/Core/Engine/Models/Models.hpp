#pragma once

#include <HeadersBase.hpp>

class Models {
    public:
        Models();
        virtual ~Models();

        virtual std::vector<float> getVertices();
        virtual std::vector<unsigned int> getIndices();
        virtual int getNbVerts();
        virtual int getNbIndices();

        virtual void InitVerticesAndIndices() = 0;
        virtual void bindBuffers() = 0;
        virtual void Init();
        virtual void Update() = 0;
        virtual void Clear();

        virtual glm::vec3 getBBmin();
        virtual glm::vec3 getBBmax();

    protected:
        unsigned int EBO, VBO, VAO;
        std::vector<unsigned int> indices;
        std::vector<float> vertices;
        glm::vec3 minAABB, maxAABB;
};

