#pragma once

#include <Models.hpp>
#include <ModelManager.hpp>

class Sphere : public Models {
    public:
        Sphere(int gridSize);
        void InitVerticesAndIndices() override;
        void bindBuffers() override;
        void Update() override;

    private:
        static const bool registered;
        int gridSize;
};
