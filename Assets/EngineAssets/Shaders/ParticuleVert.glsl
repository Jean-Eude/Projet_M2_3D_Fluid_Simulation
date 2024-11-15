#version 460 core

layout (location = 0) in vec3 aPos;


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

uniform mat4 mvp;

void main() {
    vec3 position = particles[gl_VertexID].pos; // Lecture depuis le SSBO
    gl_Position = mvp * vec4(position, 1.0);
}
