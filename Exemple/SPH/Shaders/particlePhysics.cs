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

uniform float deltaTime;

uniform int particleCount;
uniform float particleRestDensity; // 1000.0
uniform float particleMass; // 1.0
uniform float particleViscosity; // 0.01
uniform float stiffness; // 1.0
uniform float smoothingLength; // Taille du noyau
uniform vec3 gravity;

// première dérivée du Spiky kernel pour la pression (ok)
float dWSpiky(float r, float h) {
    if (r <= 0.0 || r > h) {
        return 0.0;
    }
    float h6 = pow(h, 6);
    float c = h - r;
    float c2 = c * c;
    return -45.0 / (M_PI * h6) * c2 / max(r, 1e-6);
}

// seconde dérivée du kernel de viscosité (laplacien)
float d2WLaplacian(float r, float h) {
    if (0 > r || r > h) {
        return 0.0;
    }
    float h2 = h * h;
    float h6 = h2 * h2 * h2;
    return 45.0 / (M_PI * h6) * (h - r);
}

// Equation d'état linéaire simplifiée

float getPressurePoint(float density) {
    return max(stiffness * (density - particleRestDensity), 0.0);
}

/*
// Equation de Tait aussi pour c
float getPressurePoint(float density) {
    const float gamma = 7.0; // Exposant pour l'eau
    return stiffness * (pow(density / particleRestDensity, gamma) - 1.0);
}*/


// https://wickedengine.net/2018/05/scalabe-gpu-fluid-simulation/comment-page-1/

vec3 getPressureForce(Particule p) {
    uint id = gl_GlobalInvocationID.x;
    vec3 pressureForce = vec3(0.0);
    float particlePressure = getPressurePoint(p.density);
    for (int i = 0; i < particleCount; ++i) {
        vec3 particleDistance = p.pos - particles[i].pos;
        float radius2 = dot(particleDistance, particleDistance);

        if (radius2 > smoothingLength * smoothingLength || radius2 < 1e-6) {
            continue;
        }

        float radius = sqrt(radius2);      
        vec3 direction = particleDistance / radius;

        float safeDensity = max(p.density, 1e-6);
        float neighborDensity = max(particles[i].density, 1e-6);

        float neighborPressure = getPressurePoint(particles[i].density);
        pressureForce -= particleMass * ((particlePressure / (safeDensity * safeDensity)) + (neighborPressure / (neighborDensity * neighborDensity))) * dWSpiky(radius, smoothingLength) * direction;
    }

    return pressureForce;
}

vec3 getViscosityForce(Particule p) {
    vec3 viscosityForce = vec3(0.0);
    for (int i = 0; i < particleCount; ++i) {
        vec3 particleDistance = p.pos - particles[i].pos;
        float radius2 = dot(particleDistance, particleDistance);

        if (radius2 > smoothingLength * smoothingLength || radius2 < 1e-6) {
            continue;
        }

        float radius = sqrt(radius2);

        float neighborDensity = max(particles[i].density, 1e-6);

        viscosityForce += particleMass * (particles[i].velocity - p.velocity) / neighborDensity * d2WLaplacian(radius, smoothingLength);
    }

    return particleViscosity * viscosityForce;
}

vec3 getGravityForce(Particule p) {
    //return vec3(0.0, 25.0 * gravity, 0);
    return 25.0 * gravity;
}

vec3 getAppliedForce(Particule p) {
    float safeDensity = max(p.density, 1e-6);
    return (getPressureForce(p) + getViscosityForce(p) + getGravityForce(p)) / safeDensity;
}

void main() {
    uint id = gl_GlobalInvocationID.x;

    // Vérifie si l'ID dépasse le nombre de particules (comme en CUDA)
    if (id >= particles.length()) return;

    Particule p = particles[id];

    // ------------------------------------------------------------------ //

    p.force = getAppliedForce(p);

    // ------------------------------------------------------------------ //

    // Stocker les changements dans le buffer
    particles[id] = p;
}