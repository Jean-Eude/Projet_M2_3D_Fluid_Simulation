#version 460 core

in vec3 position;
out vec4 FragColor;

uniform mat4 view;         // Matrice vue
uniform float minDepth;    // Profondeur minimale
uniform float maxDepth;    // Profondeur maximale
uniform vec2 ScreenSize;   // Taille de l'écran (largeur, hauteur)

float bandMin = 0.0;       // Limite inférieure du Narrow Band
float bandMax = 1.0;       // Limite supérieure du Narrow Band

void main() {
    vec2 texelSize = 1.0 / ScreenSize;
    vec4 viewPos = view * vec4(position, 1.0);
    float linearDepth = -viewPos.z;
    float constrainedDepth = clamp((linearDepth - minDepth) / (maxDepth - minDepth), 0.0, 1.0);

    if (constrainedDepth < bandMin || constrainedDepth > bandMax) {
        // Hors du Narrow Band, profondeur ignorée (transparent)
        FragColor = vec4(0.0, 0.0, 0.0, 0.0);
        return;
    }

    float narrowDepth = (constrainedDepth - bandMin) / (bandMax - bandMin);

    vec2 fragCoord = gl_FragCoord.xy;
    vec2 uv = fragCoord / ScreenSize;

    float depthRight = clamp(((-view * vec4(position + vec3(texelSize.x, 0.0, 0.0), 1.0)).z - minDepth) / (maxDepth - minDepth), 0.0, 1.0);
    float depthUp = clamp(((-view * vec4(position + vec3(0.0, texelSize.y, 0.0), 1.0)).z - minDepth) / (maxDepth - minDepth), 0.0, 1.0);

    vec3 dX = vec3(texelSize.x, 0.0, depthRight - constrainedDepth);
    vec3 dY = vec3(0.0, texelSize.y, depthUp - constrainedDepth);

    vec3 normal = normalize(cross(dX, dY));

    FragColor = vec4(0.5 * (normal + 1.0), 1.0);
}
