#version 460 core

layout(local_size_x = 256) in;

// Alignement sur 16 bytes pour respecter std430
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

uniform float deltaTime;          // Temps écoulé depuis la dernière frame (en secondes)
uniform vec3 minAABB;             // Bornes minimales de la boîte
uniform vec3 maxAABB;             // Bornes maximales de la boîte
uniform float globalTime;         // Temps global (en secondes)

// Paramètres du canon
uniform vec3 canonPosition;       // Position du canon
uniform vec3 canonDirection;      // Direction principale
uniform float emissionRate;       // Particules émises par seconde
uniform float particleLifetime;   // Durée de vie des particules (en secondes)
uniform float speed;              // Vitesse initiale des particules
uniform float dispersion;         // Dispersion autour de la direction

#define RESTITUTION 0.3           // Coefficient de restitution pour les collisions

void main() {
    uint id = gl_GlobalInvocationID.x;

    // Vérifie si l'ID dépasse le nombre de particules
    if (id >= particles.length()) return;

    Particule p = particles[id];

    if (p.isActive == 0) {
        float emissionInterval = 1.0 / emissionRate; 
        float startTime = id * emissionInterval;

        if (globalTime >= startTime) {
            p.isActive = 1;
            p.life = particleLifetime;
            p.pos = canonPosition;

            vec3 randomDir = normalize(canonDirection + dispersion * vec3(
                fract(sin(dot(vec3(id, id + 1, id + 2), vec3(12.9898, 78.233, 37.719))) * 43758.5453) - 0.5,
                fract(sin(dot(vec3(id + 3, id + 4, id + 5), vec3(93.9898, 67.345, 47.112))) * 43758.5453) - 0.5,
                fract(sin(dot(vec3(id + 6, id + 7, id + 8), vec3(15.784, 56.124, 91.783))) * 43758.5453) - 0.5
            ));

            p.velocity = randomDir * speed;
        }
    }

    if (p.isActive == 1) {
        p.velocity += p.force * deltaTime;
        p.pos += p.velocity * deltaTime;

        p.life -= deltaTime;
        if (p.life <= 0.0) {
            p.isActive = 0;
        }
    }

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
