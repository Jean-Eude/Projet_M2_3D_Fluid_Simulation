#version 460 core

layout (location = 0) in vec3 aPos;  // Position des sommets
layout (location = 1) in vec2 aTex;  // Coordonnées de texture
layout (location = 2) in vec3 aNorm; // Normales des sommets

uniform mat4 mvp;

void main() {
    gl_Position = mvp * vec4(aPos, 1.0);
}