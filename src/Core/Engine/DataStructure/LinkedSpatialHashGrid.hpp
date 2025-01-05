#pragma once

#include <HeadersBase.hpp>

class Particule;
class EngineManager;

class LinkedSpatialHashGrid {
public:

    struct Bucket {
        unsigned pointer;
        unsigned size;
    };

    static void initShaders(EngineManager& manager, const std::vector<Particule>& particles, const glm::vec3& min, const glm::vec3& max, unsigned gridSize, int numGroupsX, int numGroupsY, int numGroupsZ);

    static void buildParticlesInteraction(EngineManager& manager, const std::vector<Particule>& particles, const glm::vec3& min, const glm::vec3& max, unsigned gridSize);

};