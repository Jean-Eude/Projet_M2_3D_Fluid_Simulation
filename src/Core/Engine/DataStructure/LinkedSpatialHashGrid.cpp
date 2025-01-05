#include <LinkedSpatialHashGrid.hpp>
#include <EngineManager.hpp>

static std::vector<LinkedSpatialHashGrid::Bucket> grid;
static std::vector<unsigned> buckets;

void LinkedSpatialHashGrid::initShaders(EngineManager& manager, const std::vector<Particule>& particles, const glm::vec3& min, const glm::vec3& max, unsigned gridSize, int numGroupsX, int numGroupsY, int numGroupsZ) {
    manager.shaders.enqueueComputeShader("countEntriesCS", FilePath::getFilePath("/Assets/EngineAssets/Shaders/countHashGridEntries.glsl"));
    manager.shaders.setNumGroupsComputeShaderByName("countEntriesCS", numGroupsX, numGroupsY, numGroupsZ, particles.size(), 1, 1);

    manager.shaders.useComputeShaderByName("countEntriesCS");
    manager.shaders.setCompBind3f("countEntriesCS", "minAABB", min);
    manager.shaders.setCompBind3f("countEntriesCS", "maxAABB", max);
    manager.shaders.setCompBind1u("countEntriesCS", "gridSize", gridSize);

    manager.shaders.enqueueComputeShader("putEntriesCS", FilePath::getFilePath("/Assets/EngineAssets/Shaders/putHashGridEntries.glsl"));
    manager.shaders.setNumGroupsComputeShaderByName("putEntriesCS", numGroupsX, numGroupsY, numGroupsZ, particles.size(), 1, 1);

    manager.shaders.useComputeShaderByName("putEntriesCS");
    manager.shaders.setCompBind3f("putEntriesCS", "minAABB", min);
    manager.shaders.setCompBind3f("putEntriesCS", "maxAABB", max);
    manager.shaders.setCompBind1u("putEntriesCS", "gridSize", gridSize);

    unsigned bucketCount = gridSize * gridSize * gridSize;
    grid.resize(bucketCount);
    buckets.resize(particles.size());

    manager.ssboM.enqueueSSBO("gridSSBO", GL_DYNAMIC_DRAW, grid);
    manager.ssboM.enqueueSSBO("bucketsSSBO", GL_DYNAMIC_DRAW, buckets);
}

void LinkedSpatialHashGrid::buildParticlesInteraction(EngineManager& manager, const std::vector<Particule>& particles, const glm::vec3& min, const glm::vec3& max, unsigned gridSize) {
    unsigned bucketCount = gridSize * gridSize * gridSize;

    manager.ssboM.bindBufferBaseByName("gridSSBO");
    manager.ssboM.bindBufferBaseByName("bucketsSSBO");

    std::fill(grid.begin(), grid.end(), Bucket{0u, 0u});
    manager.ssboM.UpdateSBOByName("gridSSBO", grid);

    manager.shaders.useComputeShaderByName("countEntriesCS");
    manager.shaders.memoryBarrierByName("countEntriesCS", CS_SSBO);
    manager.ssboM.linkSSBOByName("gridSSBO", GL_READ_ONLY, grid);

    unsigned offset = 0;
    for (unsigned i = 0; i < bucketCount; ++i) {
        grid[i].pointer = offset;
        offset += grid[i].size;
        grid[i].size = 0;
    }

    manager.ssboM.UpdateSBOByName("gridSSBO", grid);

    manager.shaders.useComputeShaderByName("putEntriesCS");
    manager.shaders.memoryBarrierByName("putEntriesCS", CS_SSBO);
}