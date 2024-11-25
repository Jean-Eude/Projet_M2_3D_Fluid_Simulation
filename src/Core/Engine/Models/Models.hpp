#pragma once

#include <HeadersBase.hpp>

class Models {
    public:
        Models();
        ~Models();

        virtual std::vector<unsigned int>getVertices();
        virtual std::vector<unsigned int>getIndices();
        virtual int getNbVerts();
        virtual int getNbIndices();

        virtual void Init();
        virtual void Update();
        virtual void Clear();

    protected:
        unsigned int EBO, VBO, VAO;
        std::vector<unsigned int> indices;
        std::vector<float> vertices;
};
