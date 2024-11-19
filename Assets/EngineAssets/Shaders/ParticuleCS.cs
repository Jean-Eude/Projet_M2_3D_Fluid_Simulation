#version 460 core

layout(local_size_x = 256) in;

// Alignement sur 16 bytes, 32, etc... pour une structure dans les shaders
struct Particule {
    vec3 pos; 
    vec3 velocity;
    vec3 dir;     
    float scale;   
    float life;    

    float padding1;
    float padding2;
    float padding3;
};

layout(std430, binding = 0) buffer ParticuleBuffer {
    Particule particles[];
};

uniform float deltaTime;

void main() {
    uint id = gl_GlobalInvocationID.x;

    // Vérifie si l'ID dépasse le nombre de particules (comme en CUDA)
    if (id >= particles.length()) return;

    Particule p = particles[id];

    // ------------------------------------------------------------------ //

    

    // ------------------------------------------------------------------ //

    // Stocker les changements dans le buffer
    particles[id] = p;
}
