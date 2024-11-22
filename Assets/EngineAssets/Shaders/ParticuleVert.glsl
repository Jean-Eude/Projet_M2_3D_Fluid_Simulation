#version 460 core

layout (location = 0) in vec3 aPos;

out vec4 couleur;
out float density;
out vec3 velocity;
out vec3 force;

struct Particule {
    vec3 pos;
    float _pad1; 
    vec3 velocity;
    float _pad2;
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

uniform mat4 mvp;

uniform vec3 minAABB;
uniform vec3 maxAABB;

uniform vec3 camPos;

uniform float tailleParticule;


void main() {
    vec3 position = particles[gl_VertexID].pos; // Lecture depuis le SSBO
    gl_PointSize = tailleParticule;
    gl_Position = mvp * vec4(position, 1.0);

    // Normalisation de la position pour la couleur (dans l'intervalle [0, 1])
    vec3 normalizedPos = (position - minAABB) / (maxAABB - minAABB);
    // Couleur basée sur la position normalisée
    couleur = vec4(vec3(particles[gl_VertexID].density), 1.0f);
    velocity = particles[gl_VertexID].velocity;
    density = particles[gl_VertexID].density;
    force = particles[gl_VertexID].force;
}
