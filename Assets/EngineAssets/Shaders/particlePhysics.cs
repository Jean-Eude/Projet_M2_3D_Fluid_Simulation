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
    float scale;         
    float life;
    float density;
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

// première dérivée du Spiky kernel pour la pression
float dWSpiky(float r, float h) {
    if (0 > r && r > h) {
        return 0.0;
    }
    float h2 = h * h;
    float h6 = h2 * h2 * h2;
    float absr = abs(r);
    float c = h - absr;
    float c2 = c * c;
    return 45.0 / (h6 * M_PI * absr) * c2;
}

float wSpiky(float r, float h) {
    if (0 > r && r > h) {
        return 0.0;
    }
    float h2 = h * h;
    float h6 = h2 * h2 * h2;
    float hr = (h - r);
    float hr3 = hr * hr * hr;
    return 15.0 / (h6 * M_PI) * hr;
}

float wLaplacian(float r, float h) {
    if (0 > r && r > h) {
        return 0.0;
    }
    float h2 = h * h;
    float h3 = h * h2;
    float r2 = r * r;
    float r3 = r2 * r;
    return 15.0 / (2 * M_PI * h3) * (-r3 / (2.0 * h3) + r2 / h2 + h / (2.0 * r) - 1);
}

// seconde dérivée du kernel de viscosité (laplacien)
float d2WLaplacian(float r, float h) {
    if (0 > r && r > h) {
        return 0.0;
    }
    float h2 = h * h;
    float h6 = h2 * h2 * h2;
    return 45.0 / (h6 * M_PI) * (h - abs(r));
}

float getPressurePoint(float density) {
    return max(stiffness * (density - particleRestDensity), 0.0);
}

vec3 getPressureForce(Particule p) {
    uint id = gl_GlobalInvocationID.x;
    vec3 pressureForce = vec3(0.0);
    float particlePressure = getPressurePoint(p.density);
    for (int i = 0; i < particleCount; ++i) {
        vec3 particleDistance = particles[i].pos - p.pos;
        float radius = length(particleDistance);
        pressureForce -= particleMass / particles[i].density * getPressurePoint(particles[i].density) * wSpiky(radius, smoothingLength) * particleDistance;
    }
    return pressureForce;
}

vec3 getViscosityForce(Particule p) {
    vec3 viscosityForce = vec3(0.0);
    for (int i = 0; i < particleCount; ++i) {
        float radius = length(particles[i].pos - p.pos);
        viscosityForce += particleMass / particles[i].density * particles[i].pos - p.pos * d2WLaplacian(radius, smoothingLength);
    }
    return particleViscosity * viscosityForce;
}

vec3 getGravityForce(Particule p) {
    return p.density * vec3(0.0, 9.81, 0);
}

vec3 getAppliedForce(Particule p) {
    return getPressureForce(p) + getViscosityForce(p) + getGravityForce(p);
}

void main() {
    uint id = gl_GlobalInvocationID.x;

    // Vérifie si l'ID dépasse le nombre de particules (comme en CUDA)
    if (id >= particles.length()) return;

    Particule p = particles[id];

    // ------------------------------------------------------------------ //

    //p.velocity += getAppliedForce(p) * deltaTime;
    p.velocity = vec3(1,0,0);
    //p.pos += p.velocity * deltaTime;

    // ------------------------------------------------------------------ //

    // Stocker les changements dans le buffer
    particles[id] = p;
}
