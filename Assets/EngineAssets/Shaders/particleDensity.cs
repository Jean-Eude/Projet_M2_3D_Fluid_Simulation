#version 460 core

#define M_PI 3.1415926535897932384626433832795

layout(local_size_x = 256) in;

// Alignement sur 16 bytes, 32, etc... pour une structure dans les shaders
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

struct Bucket {
    uint pointer;
    uint size;
};

layout(std430, binding = 0) buffer ParticuleBuffer {
    Particule particles[];
};

layout(std430, binding = 1) buffer GridBuffer {
    Bucket grid[];
};

layout(std430, binding = 2) buffer BucketBuffer {
    uint buckets[];
};

uniform int particleCount;
uniform float particleMass; // 1.0
uniform float smoothingLength; // Taille du noyau

uniform vec3 minAABB;
uniform vec3 maxAABB;
uniform uint gridSize;

ivec3 cell(vec3 position) {
    return clamp(ivec3((clamp(position, minAABB, maxAABB) - minAABB) / (maxAABB - minAABB) * float(gridSize)), ivec3(0), ivec3(gridSize - 1));
}

uint cellHash(uint x, uint y, uint z) {
    return clamp(x, 0, gridSize - 1) + gridSize * (clamp(y, 0, gridSize - 1) + gridSize * clamp(z, 0, gridSize - 1));
}

// kernel Poly6 pour la densité (ok)
float wPoly6(float r, float h) {
    if (0 > r || r > h) {
        return 0.0;
    }
    float h2 = h * h;
    float h9 = h2 * h2 * h2 * h2 * h;
    float c = h2 - r * r;
    float c3 = c * c * c;
    return 315.0 / (64.0 * M_PI * h9) * c3;
}

float getDensity(Particule p) {
    vec3 bmin = clamp(p.pos - vec3(smoothingLength), minAABB, maxAABB);
    vec3 bmax = clamp(p.pos + vec3(smoothingLength), minAABB, maxAABB);
    ivec3 cmin = cell(bmin);
    ivec3 cmax = cell(bmax);
    float density = 0.0;

    for (uint x = cmin.x; x <= cmax.x; ++x) {
        for (uint y = cmin.y; y <= cmax.y; ++y) {
            for (uint z = cmin.z; z <= cmax.z; ++z) {
                uint bucketHash = cellHash(x, y, z);
                Bucket bucket = grid[bucketHash];

                for (uint l = 0; l < bucket.size; ++l) {
                    uint id2 = buckets[bucket.pointer + l];

                    vec3 diff = particles[id2].pos - p.pos;  
                    float radius2 = dot(diff, diff);      
                    if (radius2 > smoothingLength * smoothingLength) {
                        continue;
                    }
                    float radius = sqrt(radius2);        
                    density += particleMass * wPoly6(radius, smoothingLength);
                }
            }
        }
    }
    return density;
}

void main() {
    uint id = gl_GlobalInvocationID.x;

    // Vérifie si l'ID dépasse le nombre de particules (comme en CUDA)
    if (id >= particles.length()) return;

    Particule p = particles[id];

    p.density = getDensity(p);

    particles[id] = p;
}