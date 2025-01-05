#pragma once

#include <Models.hpp>
#include <ModelManager.hpp>

class Plane : public Models {
    public:
        Plane(float size, int div, bool isBottom);
        void InitVerticesAndIndices() override;
        void bindBuffers() override;
        void Update() override;

    private:
        static const bool registered;
        int div;
        float size;
        bool isBottom;
};
