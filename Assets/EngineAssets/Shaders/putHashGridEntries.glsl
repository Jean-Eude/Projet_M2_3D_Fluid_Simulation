#version 460 core

layout(local_size_x = 256) in;

struct Bucket {
    uint pointer;
    uint size;
};

layout(std430, binding = 1) buffer HashBuffer {
    uint hashes[];
};

layout(std430, binding = 3) buffer GridBuffer {
    Bucket grid[];
};

layout(std430, binding = 4) buffer BucketBuffer {
    uint buckets[];
};

void main() {
    uint id = gl_GlobalInvocationID.x;

    if (id >= hashes.length()) return;

    uint h = hashes[id];
        
    Bucket b = grid[h];
        
    buckets[b.pointer + atomicAdd(grid[h].size, 1)] = id;
}