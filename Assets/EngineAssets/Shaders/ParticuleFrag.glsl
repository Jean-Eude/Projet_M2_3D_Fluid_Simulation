#version 460 core

in vec4 couleur;
out vec4 FragColor;

void main() {
    // SDF pour une sphère
    vec2 coord = gl_PointCoord - vec2(0.5);
    float dist = length(coord);

    if (dist > 0.5) { 
        discard;
    }

    FragColor = couleur;
}