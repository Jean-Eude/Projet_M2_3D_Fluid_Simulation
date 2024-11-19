#version 460 core

layout(local_size_x = 256) in;

struct Particule {
    vec3 pos;
    vec3 dir;
    vec3 velocity;
    float scale;
    float life;
};

layout(std430, binding = 0) buffer ParticuleBuffer {
    Particule particles[];
};

uniform float deltaTime;  // Temps écoulé entre deux mises à jour
uniform vec3 gravity;     // Force gravitationnelle
uniform vec3 spawnPos;    // Position de la source
uniform vec3 initialVelocity; // Vitesse initiale des particules
uniform float lifeSpan;   // Durée de vie des particules

// Fonction de génération de direction aléatoire normalisée
vec3 generateRandomDirection(uint id) {
    float angle = fract(sin(float(id) * 12.9898) * 43758.5453) * 6.28318530718; // 0 à 2π
    float height = fract(cos(float(id) * 78.233) * 43758.5453) * 2.0 - 1.0;    // [-1, 1]
    vec3 randomDir = vec3(cos(angle), height, sin(angle));
    return normalize(randomDir);
}

// Génération d'une durée de vie aléatoire
float generateRandomLife(uint id) {
    return fract(sin(float(id) * 13.731) * 43758.5453) * lifeSpan;
}

void main() {
    uint id = gl_GlobalInvocationID.x;

    // Vérifier si l'ID dépasse le nombre de particules (comme en CUDA)
    if (id >= particles.length()) return;

    Particule p = particles[id];

    // ------------------------------------------------------------------ //


    // Mise à jour de la durée de vie
    p.life -= deltaTime;

    // Si la particule est "morte", réinitialisation
    if (p.life <= 0.0) {
        // Réinitialisation de la position
        p.pos = spawnPos;

        // Génération d'une direction aléatoire normalisée
        vec3 randomDir = generateRandomDirection(id);

        // Initialisation de la vitesse avec une légère variation
        p.velocity = initialVelocity + randomDir * 2.0;

        // Génération d'une nouvelle durée de vie
        p.life = generateRandomLife(id);
    } else {
        // Mise à jour de la position et de la vitesse avec intégration d'Euler
        p.velocity += gravity * deltaTime; // Mise à jour de la vitesse (accélération gravitationnelle)
        p.pos += p.velocity * deltaTime;  // Mise à jour de la position
    }


    // ------------------------------------------------------------------ //

    // Stocker les changements dans le buffer
    particles[id] = p;
}
