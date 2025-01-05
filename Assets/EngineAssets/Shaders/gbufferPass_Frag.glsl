#version 460 core

layout (location = 0) out vec3 aPos;
layout (location = 1) out vec2 aTex;
layout (location = 2) out vec3 aNorm;

in vec3 FragPos;
in vec2 TexCoords;
in vec3 Normal;

void main() {    
    aPos = FragPos;
    aNorm = normalize(Normal);
}