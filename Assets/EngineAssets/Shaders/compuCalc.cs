#version 460 core

layout(local_size_x = 16) in;

layout(std430, binding = 0) buffer DataBuffer {
    float data[];
};

layout(std430, binding = 1) buffer DataBuffer2 {
    float data2[];
};

void main() {
    uint id = gl_GlobalInvocationID.x;
    data[id] *= 2;
    data2[id] *= 2;
}