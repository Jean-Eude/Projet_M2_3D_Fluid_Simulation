#pragma once

#include <Models.hpp>
#include <ModelManager.hpp>

class Plane : public Models {
public:
    Plane(int gridSize);
    void InitVerticesAndIndices() override;
    void bindBuffers() override;
    void Update() override;

private:
    static const bool registered;
    int gridSize;
};
