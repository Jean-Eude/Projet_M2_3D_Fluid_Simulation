#version 460 core

layout(local_size_x = 256) in;

struct Particule {
    vec3 pos;
    float _pad1; 
    vec3 velocity;
    float _padd2;
    vec3 dir;
    float _pad3;
    vec3 force;
    float _pad4;
    float scale;         
    float life;
    float density;
    int isActive;
};

layout(std430, binding = 0) buffer ParticuleBuffer {
    Particule particles[];
};

layout(std430, binding = 1) buffer HashBuffer {
    uint hashBuffer[];
};

layout(std430, binding = 2) buffer CounterBuffer {
    uint counters[];
};

uniform vec3 minAABB;
uniform vec3 maxAABB;
uniform uint gridSize;

/*uint hash(vec3 position) {
    vec3 p = gridSize * clamp((position - minAABB) / maxAABB, vec3(0.0), vec3(1.0));
    return uint(floor(p.x)) + gridSize * (uint(floor(p.y)) + gridSize * uint(floor(p.z)));
}*/

uint hash(vec3 position) {
    ivec3 p = clamp(ivec3((clamp(position, minAABB, maxAABB) - minAABB) / (maxAABB - minAABB) * float(gridSize)), ivec3(0), ivec3(gridSize - 1));
    return p.x + p.y * gridSize + p.z * gridSize * gridSize;
}

void main() {
    uint id = gl_GlobalInvocationID.x;

    // Vérifie si l'ID dépasse le nombre de particules (comme en CUDA)
    if (id >= particles.length()) return;

    uint h = hash(particles[id].pos);
    hashBuffer[id] = h;
    atomicAdd(counters[h], 1);
}