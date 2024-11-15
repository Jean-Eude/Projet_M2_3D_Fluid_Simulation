#version 460 core

layout(local_size_x = 16) in;

layout(std430, binding = 0) buffer DataBuffer {
    float data[];
};

void main() {
    uint id = gl_GlobalInvocationID.x;
    data[id] *= 1.05;
}