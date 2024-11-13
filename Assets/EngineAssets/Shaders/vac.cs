#version 460 core

layout (local_size_x = 16, local_size_y = 16, local_size_z = 1) in;

layout (rgba32f, binding = 0) uniform image2D tex0;
layout (rgba32f, binding = 1) uniform image2D tex2;

void main() {
    ivec2 coords = ivec2(gl_GlobalInvocationID.xy);

    // Alternance des couleurs pour un motif de damier
    bool isEven = (coords.x / 16 + coords.y / 16) % 2 == 0;

    // Couleur du damier
    vec4 color1 = vec4(1.0, 0.0, 0.0, 1.0); // Rouge
    vec4 color2 = vec4(0.0, 1.0, 0.0, 1.0); // Vert

    // Sélectionner la couleur selon la position dans le damier
    vec4 pixel = isEven ? color1 : color2;

    // Stocker le pixel dans les deux textures pour l'exemple
    imageStore(tex0, coords, pixel);

    // Alternance des couleurs pour un motif de damier
    isEven = (coords.x / 16 + coords.y / 16) % 2 == 0;

    // Couleur du damier
    color1 = vec4(1.0, 0.0, 0.0, 1.0); // Rouge
    color2 = vec4(0.0, 0.0, 1.0, 1.0); // Vert

    // Sélectionner la couleur selon la position dans le damier
    pixel = isEven ? color1 : color2;

    imageStore(tex2, coords, pixel);
}