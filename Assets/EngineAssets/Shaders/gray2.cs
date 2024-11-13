#version 460 core

layout (local_size_x = 16, local_size_y = 16, local_size_z = 1) in;

// ----------------------------------------------------------------------------
//
// uniforms
//
// ----------------------------------------------------------------------------

layout (rgba32f, binding = 0) uniform image2D tex1;
layout (rgba32f, binding = 1) uniform image2D tex2;

layout(std430, binding = 0) buffer DataBuffer {
    float data[];
};

// Fonction de bruit basée sur les coordonnées de pixel
float random(vec2 st) {
    return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453123);
}

void main() {
    ivec2 coords = ivec2(gl_GlobalInvocationID.xy);
    
    // Lire la couleur du pixel dans tex1
    vec4 pixelColor = vec4(1.) - imageLoad(tex1, coords);

    // Ajouter du bruit aléatoire à la couleur
    float noise = random(vec2(coords.x, coords.y));
    pixelColor += vec4(noise, noise, noise, 0.0);

    // Stocker la couleur bruitée dans tex2
    imageStore(tex2, coords, pixelColor);

    // Accès au buffer et mise à jour des données
    uint id = gl_GlobalInvocationID.x;
    if (id < data.length()) {
        data[id] *= 2;
    }
}
