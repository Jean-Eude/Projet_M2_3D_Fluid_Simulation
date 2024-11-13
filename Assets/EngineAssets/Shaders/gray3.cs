#version 460 core

layout (local_size_x = 16, local_size_y = 16, local_size_z = 1) in;

// ----------------------------------------------------------------------------
//
// uniforms
//
// ----------------------------------------------------------------------------

layout (rgba32f, binding = 2) uniform image2D tex4;

// Fonction de bruit basée sur les coordonnées de pixel
float random(vec2 st) {
    return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453123);
}

void main() {
    ivec2 coords = ivec2(gl_GlobalInvocationID.xy);
    
    // Lire la couleur du pixel dans tex1
    vec4 pixelColor = vec4(0.1, 0.1, 0.1, 1.0);

    // Stocker la couleur bruitée dans tex2
    imageStore(tex4, coords, pixelColor);

}
