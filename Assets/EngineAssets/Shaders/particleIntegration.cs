#version 460 core

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
};

layout(std430, binding = 0) buffer ParticuleBuffer {
    Particule particles[];
};

uniform float deltaTime;
uniform vec3 minAABB;
uniform vec3 maxAABB;

#define RESTITUTION 0.3

void main() {
    uint id = gl_GlobalInvocationID.x;

    // Vérifie si l'ID dépasse le nombre de particules (comme en CUDA)
    if (id >= particles.length()) return;

    Particule p = particles[id];

    p.velocity += p.force * deltaTime;
    p.pos += p.velocity * deltaTime;

    for (int i = 0; i < 3; i++) {
        if (p.pos[i] < minAABB[i]) {
            p.pos[i] = minAABB[i];
            p.velocity[i] *= -RESTITUTION;
        } else if (p.pos[i] > maxAABB[i]) {
            p.pos[i] = maxAABB[i];
            p.velocity[i] *= -RESTITUTION;
        }
    }


    particles[id] = p;
}
