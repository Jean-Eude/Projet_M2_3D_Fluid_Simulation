#pragma once

#include <Models.hpp>
#include <ModelManager.hpp>

class Skybox : public Models {
    public:
        Skybox(int skyboxSize);
        void InitVerticesAndIndices() override;
        void bindBuffers() override;
        void Update() override;

    private:
        static const bool registered;
        int skyboxSize;
};
