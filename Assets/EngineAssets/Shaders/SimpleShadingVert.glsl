#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTex;
layout(location = 2) in vec3 aNorm;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 mvp;

void main() {
    gl_Position = mvp * vec4(aPos, 1.0);
    FragPos = vec3(gl_Position);
    Normal = mat3(transpose(inverse(model))) * aNorm;
}
