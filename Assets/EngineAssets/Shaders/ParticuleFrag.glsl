#version 460 core

in vec4 couleur;
in float density;
in vec3 velocity;
in vec3 force;
in vec3 position;

out vec4 FragColor;

void main() {
    FragColor = vec4(velocity, 1.);
}