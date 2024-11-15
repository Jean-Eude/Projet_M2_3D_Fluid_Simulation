#version 430

layout (local_size_x = 512, local_size_y = 1, local_size_z = 1) in;

layout(std430, binding = 0) buffer positionSSBO {
    vec3 positions[];
};

layout(std430, binding = 1) buffer directionSSBO {
    vec3 directions[];
};

uniform float deltatime;

void main() {
    uint id = gl_GlobalInvocationID.x;

    // Mettre à jour la position des particules
    positions[id] += directions[id] * deltatime;

    // Vous pouvez également modifier la direction ici, si nécessaire
    // directions[id].y -= 9.81 * deltatime; // Exemple de gravité
}
