#version 460 core

layout (local_size_x = 16, local_size_y = 16, local_size_z = 1) in;

// ----------------------------------------------------------------------------
//
// uniforms
//
// ----------------------------------------------------------------------------

layout (rgba32f, binding = 0) uniform image2D tex1;

// Fonction de bruit basée sur les coordonnées de pixel
float random(vec2 st) {
    return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453123);
}

void main() {
    ivec2 coords = ivec2(gl_GlobalInvocationID);
    
    // Calculer une couleur de base (par exemple, un violet)
    vec4 baseColor = vec4(1.0, 0.5, 1.0, 1.0);
    
    // Ajouter du bruit en générant une valeur aléatoire pour chaque pixel
    float noiseValue = random(vec2(coords) * 0.1); // Facteur d'échelle pour ajuster le grain du bruit
    vec4 noisyPixel = baseColor * noiseValue;

    // Stocker la couleur du pixel dans la texture de sortie
    imageStore(tex1, coords, noisyPixel);
}

