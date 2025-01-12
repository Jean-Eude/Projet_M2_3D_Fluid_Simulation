#version 460 core

in vec4 couleur;
in float density;
in vec3 velocity;
in vec3 force;
in vec3 position;

out vec4 FragColor;

uniform int shadingFormat;

void main() {
    if (shadingFormat == 1) {
        FragColor = vec4(2.0 * position, 1.);
    }
    else {
        FragColor = vec4(2.0 * velocity, 1.);
    }
}