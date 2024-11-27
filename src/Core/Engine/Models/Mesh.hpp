#pragma once

#include <Models.hpp>
#include <ModelManager.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Mesh : public Models {
public:
    Mesh(const std::string& filepath);

    void InitVerticesAndIndices() override;
    void bindBuffers() override;
    void Update() override;

private:
    std::string filepath;
    static const bool registered;

    void processNode(aiNode* node, const aiScene* scene);
    void processMesh(aiMesh* mesh, const aiScene* scene);
};