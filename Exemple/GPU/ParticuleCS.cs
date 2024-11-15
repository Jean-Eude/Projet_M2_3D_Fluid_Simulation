#version 430

layout(local_size_x = 256) in;

struct Particule {
    vec3 pos;
    vec3 dir;
    float life;
    float maxLife;
};

layout(std430, binding = 0) buffer ParticuleBuffer {
    Particule particles[];
};

uniform vec3 gravity;
uniform float deltaTime;
uniform vec3 spawnPos;

void main() {
    uint id = gl_GlobalInvocationID.x;

    // Vérifier si l'ID dépasse le nombre de particules
    if (id >= particles.length()) return;

    Particule p = particles[id];

    // Mise à jour de la durée de vie
    p.life -= deltaTime;

    // Si la particule est "morte", réinitialisation
    if (p.life <= 0.0) {
        p.pos = spawnPos;
        p.dir = vec3(0.0, 1.0, 0.0); // Direction par défaut : vers le haut
        p.life = 5.0; // Nouvelle durée de vie
        p.maxLife = p.life;
    } else {
        // Mise à jour de la position et de la direction
        p.dir += gravity * deltaTime;
        p.pos += p.dir * deltaTime;
    }

    // Stocker les changements dans le buffer
    particles[id] = p;
}
