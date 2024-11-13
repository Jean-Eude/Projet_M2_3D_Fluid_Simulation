#version 460 core

layout(local_size_x = 16) in;

struct Particules {
    vec3 pos;
    vec3 dir;
};

layout(std430, binding = 0) buffer Positions {
    Particules particles[];
};

uniform float deltaTime;

void main() {
    uint index = gl_GlobalInvocationID.x;
    particles[index].pos += particles[index].dir * deltaTime;
}
