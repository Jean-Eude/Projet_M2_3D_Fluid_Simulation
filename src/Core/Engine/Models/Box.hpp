#pragma once

#include <Models.hpp>
#include <ModelManager.hpp>

class Box : public Models {
    public:
        Box(float boxSize);
        void InitVerticesAndIndices() override;
        void bindBuffers() override;
        void Update() override;

        glm::vec3 getBBmin() override;
        glm::vec3 getBBmax() override;

    private:
        static const bool registered;
        float boxSize;
};
