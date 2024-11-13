#version 460 core

// Définir la structure 'Particle'
struct Particle {
    vec4 position;  // Position de la particule
    vec4 velocity;  // Vitesse de la particule
};

layout (local_size_x = 256) in;

// Lier le buffer à un emplacement spécifique (binding 0)
layout(std430, binding = 0) buffer Particles {
    Particle particles[];  // Tableau de particules
};

uniform float deltaTime;

void main() {
    uint id = gl_GlobalInvocationID.x;

    // Mise à jour de la position en fonction de la vitesse
    particles[id].position.xyz += particles[id].velocity.xyz * deltaTime;

    // Application de la gravité
    particles[id].velocity.y -= 9.81 * deltaTime;
}