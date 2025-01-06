#version 460 core

in vec3 position; // Position de la particule en espace monde
in float radius;
out vec4 FragColor;

uniform mat4 view;         // Matrice de vue
uniform mat4 projection;   // Matrice de projection
uniform vec2 ScreenSize;   // Taille de l'écran (largeur, hauteur)
 vec3 lightColor = vec3(0., 0., 1.);   // Couleur de la lumière
 float absorptionCoefficient = 0.5; // Coefficient d'absorption pour la loi de Beer-Lambert
float sigma = 0.1;         // Paramètre σ du noyau Gaussien

void main() {
    // Transformation espace monde → espace vue
    vec4 viewPos = view * vec4(position, 1.0);

    // Vérification si la particule est derrière la caméra
    if (viewPos.z > 0.0) {
        discard; // Ignore la particule si elle est derrière la caméra
    }

    // Calcul de la position en espace clip (après projection)
    vec4 clipPos = projection * viewPos;

    // Transformation en espace Normalized Device Coordinates (NDC)
    vec3 ndcPos = clipPos.xyz / clipPos.w;

    // Transformation en espace écran
    vec2 fragCoord = (ndcPos.xy * 0.5 + 0.5) * ScreenSize;

    // Rayon d'influence en espace écran
    float screenRadius = (radius * 0.5) / abs(viewPos.z);

    // Distance entre le pixel actuel et le centre de la particule projetée
    float distToCenter = length(gl_FragCoord.xy - fragCoord);

    // Calcul de l'épaisseur à l'aide du noyau Gaussien Gσ
    float thickness = exp(-0.5 * pow(distToCenter / screenRadius, 2.0) / (sigma * sigma));

    // Application de la loi de Beer-Lambert pour l'atténuation de lumière
    vec3 attenuation = vec3(exp(-absorptionCoefficient * thickness));

    // Calcul de la couleur finale après atténuation
    vec3 finalColor = lightColor * attenuation;

    // Sortie finale
    FragColor = vec4(finalColor, 1.0); // Alpha à 1 pour blending
}
