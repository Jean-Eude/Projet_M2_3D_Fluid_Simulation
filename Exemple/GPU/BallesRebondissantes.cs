#version 460 core

layout(local_size_x = 256) in;

struct Particule {
    vec3 pos;
    vec3 dir;
    vec3 velocity;
    float scale;  
    float life;
    float padding[2];
};

layout(std430, binding = 0) buffer ParticuleBuffer {
    Particule particles[];
};

uniform float deltaTime;  // Temps écoulé entre deux mises à jour
uniform vec3 gravity;     // Force gravitationnelle
uniform vec3 spawnPos;    // Position de la source
uniform vec3 initialVelocity; // Vitesse initiale des particules
uniform float lifeSpan;   // Durée de vie des particules

uniform vec3 minAABB;     // Limites inférieures de la boîte
uniform vec3 maxAABB;     // Limites supérieures de la boîte

// Vérifie si une particule sphérique est dans la boîte
bool isInsideBox(Particule p) {
    return (p.pos.x - p.scale >= minAABB.x && p.pos.x + p.scale <= maxAABB.x) &&
           (p.pos.y - p.scale >= minAABB.y && p.pos.y + p.scale <= maxAABB.y) &&
           (p.pos.z - p.scale >= minAABB.z && p.pos.z + p.scale <= maxAABB.z);
}

// Gère les collisions avec les murs de la boîte pour des sphères
void handleBounce(inout Particule p) {
    // Vérifie la collision sur l'axe X
    if (p.pos.x - p.scale < minAABB.x) {
        p.pos.x = minAABB.x + p.scale;   // Contraindre la position pour rester dans la boîte
        p.velocity.x *= -1.0;            // Inverser la vitesse sur X
    } else if (p.pos.x + p.scale > maxAABB.x) {
        p.pos.x = maxAABB.x - p.scale;
        p.velocity.x *= -1.0;
    }

    // Vérifie la collision sur l'axe Y
    if (p.pos.y - p.scale < minAABB.y) {
        p.pos.y = minAABB.y + p.scale;
        p.velocity.y *= -1.0;            // Inverser la vitesse sur Y
    } else if (p.pos.y + p.scale > maxAABB.y) {
        p.pos.y = maxAABB.y - p.scale;
        p.velocity.y *= -1.0;
    }

    // Vérifie la collision sur l'axe Z
    if (p.pos.z - p.scale < minAABB.z) {
        p.pos.z = minAABB.z + p.scale;
        p.velocity.z *= -1.0;            // Inverser la vitesse sur Z
    } else if (p.pos.z + p.scale > maxAABB.z) {
        p.pos.z = maxAABB.z - p.scale;
        p.velocity.z *= -1.0;
    }
}

// Génère une direction aléatoire normalisée
vec3 generateRandomDirection(uint id) {
    float angle = fract(sin(float(id) * 12.9898) * 43758.5453) * 6.28318530718; // 0 à 2π
    float height = fract(cos(float(id) * 78.233) * 43758.5453) * 2.0 - 1.0;    // [-1, 1]
    vec3 randomDir = vec3(cos(angle), height, sin(angle));
    return normalize(randomDir);
}

// Génère une durée de vie aléatoire
float generateRandomLife(uint id) {
    return fract(sin(float(id) * 13.731) * 43758.5453) * lifeSpan;
}

void main() {
    uint id = gl_GlobalInvocationID.x;

    // Vérifie si l'ID dépasse le nombre de particules
    if (id >= particles.length()) return;

    Particule p = particles[id];

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

        // Assigner un rayon (scale) aléatoire
        p.scale = 0.1 + fract(sin(float(id) * 27.89) * 43758.5453) * 0.2; // Entre 0.1 et 0.3
    } else {
        // Mise à jour de la position et de la vitesse avec intégration d'Euler
        p.velocity += gravity * deltaTime; // Mise à jour de la vitesse (accélération gravitationnelle)
        p.pos += p.velocity * deltaTime;  // Mise à jour de la position

        // Gérer les rebonds sur les murs de la boîte
        handleBounce(p);
    }

    // Stocker les changements dans le buffer
    particles[id] = p;
}
