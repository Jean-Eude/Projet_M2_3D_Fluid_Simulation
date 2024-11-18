#version 460 core

layout (location = 0) in vec3 aPos;

out vec4 couleur;

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

uniform mat4 mvp;

uniform vec3 minAABB;
uniform vec3 maxAABB;

void main() {
    vec3 position = particles[gl_VertexID].pos; // Lecture depuis le SSBO

    // Normalisation de la position pour la couleur (dans l'intervalle [0, 1])
    vec3 normalizedPos = (position - minAABB) / (maxAABB - minAABB);
    // Couleur basée sur la position normalisée
    couleur = vec4(normalizedPos, 1.0f);
    
    gl_PointSize = 10.;
    gl_Position = mvp * vec4(position, 1.0);
}
