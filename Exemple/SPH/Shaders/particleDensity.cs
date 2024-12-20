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

layout(std430, binding = 0) buffer ParticuleBuffer {
    Particule particles[];
};

uniform int particleCount;
uniform float particleMass; // 1.0
uniform float smoothingLength; // Taille du noyau

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
    float density = 0.0;
    for (int i = 0; i < particleCount; ++i) {
        vec3 diff = particles[i].pos - p.pos;  
        float radius2 = dot(diff, diff);      
        if (radius2 > smoothingLength * smoothingLength) continue; 
        float radius = sqrt(radius2);        
        density += particleMass * wPoly6(radius, smoothingLength);
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