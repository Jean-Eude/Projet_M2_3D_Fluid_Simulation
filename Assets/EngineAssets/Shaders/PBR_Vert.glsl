#version 460 core

layout (location = 0) in vec3 aPos;  // Position des sommets
layout (location = 1) in vec2 aTex;  // Coordonnées de texture
layout (location = 2) in vec3 aNorm; // Normales des sommets

uniform mat4 mvp;    // Matrice Model-View-Projection
uniform mat4 model;  // Matrice de transformation du modèle
uniform mat4 projection;
uniform mat4 view;

out vec3 Pos; 
out vec2 Tex;         
out vec3 Normal;   

void main() {
    Pos = vec3(model * vec4(aPos, 1.0));
    Tex = aTex;
    Normal = normalize(mat3(transpose(inverse(model))) * aNorm); 
    
    gl_Position = projection * view * vec4(Pos, 1.0);
}
