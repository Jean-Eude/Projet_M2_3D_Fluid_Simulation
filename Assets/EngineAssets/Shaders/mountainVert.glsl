#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

uniform mat4 mvp;

out vec3 pos;

float random(vec2 st) {
    return fract(sin(dot(st.xy, vec2(12.9898,78.233))) * 43758.5453123);
}

float noise(vec2 st) {
    vec2 i = floor(st); // Coordonnées de cellule
    vec2 f = fract(st); // Fraction de la cellule

    float a = random(i);
    float b = random(i + vec2(1.0, 0.0));
    float c = random(i + vec2(0.0, 1.0));
    float d = random(i + vec2(1.0, 1.0));

    vec2 u = f * f * (3.0 - 2.0 * f);
    return mix(a, b, u.x) + (c - a) * u.y * (1.0 - u.x) + (d - b) * u.x * u.y;
}


void main()
{
    pos = aPos;
    float displacement = noise(aPos.xz * 2.0);
    pos.y += displacement * 1.; 

    gl_Position = mvp * vec4(pos, 1.0); // Calcul de la position finale
}