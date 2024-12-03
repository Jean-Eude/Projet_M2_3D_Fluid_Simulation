#include <LinkedSpatialHashGrid.hpp>
#include <EngineManager.hpp>

/*LinkedSpatialHashGrid::LinkedSpatialHashGrid(const glm::vec3& min, const glm::vec3& max, unsigned gridSize, const std::vector<Bucket>& grid, const std::vector<unsigned>& buckets) :
    min{min}, max{max}, gridSize{gridSize}, grid{grid}, buckets{buckets}
{}*/

static std::vector<unsigned> hashes;
static std::vector<unsigned> counters;
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

    unsigned bucketCount = gridSize * gridSize * gridSize;
    hashes.resize(particles.size());
    counters.resize(bucketCount, 0);
    grid.resize(bucketCount);
    buckets.resize(particles.size());

    manager.ssboM.enqueueSSBO("hashesSSBO", GL_DYNAMIC_DRAW, hashes);
    manager.ssboM.enqueueSSBO("countersSSBO", GL_DYNAMIC_DRAW, counters);
    manager.ssboM.enqueueSSBO("gridSSBO", GL_DYNAMIC_DRAW, grid);
    manager.ssboM.enqueueSSBO("bucketsSSBO", GL_DYNAMIC_DRAW, buckets);
}

void LinkedSpatialHashGrid::buildParticlesInteraction(EngineManager& manager, const std::vector<Particule>& particles, const glm::vec3& min, const glm::vec3& max, unsigned gridSize) {
    unsigned bucketCount = gridSize * gridSize * gridSize;

    manager.ssboM.bindBufferBaseByName("hashesSSBO");
    manager.ssboM.bindBufferBaseByName("countersSSBO");
    manager.ssboM.bindBufferBaseByName("gridSSBO");
    manager.ssboM.bindBufferBaseByName("bucketsSSBO");

    std::fill(counters.begin(), counters.end(), 0);
    manager.ssboM.UpdateSBOByName("countersSSBO", counters);

    manager.shaders.useComputeShaderByName("countEntriesCS");
    manager.shaders.memoryBarrierByName("countEntriesCS", CS_SSBO);
    manager.ssboM.linkSSBOByName("hashesSSBO", GL_READ_ONLY, hashes);
    manager.ssboM.linkSSBOByName("countersSSBO", GL_READ_ONLY, counters);

    unsigned offset = 0;
    for (unsigned i = 0; i < bucketCount; ++i) {
        grid[i].pointer = offset;
        grid[i].size = 0;
        offset += counters[i];
    }

    manager.ssboM.UpdateSBOByName("gridSSBO", grid);

    manager.shaders.useComputeShaderByName("putEntriesCS");
    manager.shaders.memoryBarrierByName("putEntriesCS", CS_SSBO);
    //manager.ssboM.linkSSBOByName("gridSSBO", GL_READ_ONLY, grid); // pas necessaire, en dessous aussi
    //manager.ssboM.linkSSBOByName("bucketsSSBO", GL_READ_ONLY, buckets);
}