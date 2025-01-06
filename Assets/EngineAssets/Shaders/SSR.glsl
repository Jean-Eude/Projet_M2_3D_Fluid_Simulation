#version 460 core

in vec3 position; // Position de la particule en espace monde
in float radius;
out vec4 FragColor;

uniform mat4 view;          
uniform mat4 projection;     
uniform vec2 ScreenSize;     
uniform vec3 baseColor;                
uniform float absorptionCoefficient;  // Coefficient d'absorption
uniform float minDepth;               // Profondeur minimale
uniform float maxDepth;               // Profondeur maximale
uniform float sigma;                  // Paramètre σ du noyau Gaussien

void main() {
    // ** 1. Transformation en espace vue **
    vec4 viewPos = view * vec4(position, 1.0);

    // Vérification si la particule est derrière la caméra
    if (viewPos.z > 0.0) {
        discard;
    }

    // ** 2. Transformation en espace clip et écran **
    vec4 clipPos = projection * viewPos;
    vec3 ndcPos = clipPos.xyz / clipPos.w; // Normalized Device Coordinates (NDC)
    vec2 fragCoord = (ndcPos.xy * 0.5 + 0.5) * ScreenSize; // Espace écran

    // ** 3. Calcul de la thickness en espace écran **
    float screenRadius = (radius * 0.5) / abs(viewPos.z); // Rayon projeté en espace écran
    float distToCenter = length(gl_FragCoord.xy - fragCoord); // Distance pixel-particule
    float thickness = exp(-0.5 * pow(distToCenter / screenRadius, 2.0) / (sigma * sigma));

    thickness = clamp(thickness, 0.0, 1.0);
    float amplifiedThickness = thickness * 5.0;

    // ** 4. Loi de Beer-Lambert pour l'absorption **
    float absorption = exp(-absorptionCoefficient * amplifiedThickness);
    vec3 finalColor = baseColor * absorption;

    // ** 5. Calcul de la transparence dynamique (alpha) **
    float alpha = 1.0 - thickness;
    alpha = pow(alpha, 2.0);       

    // ** 6. Gamma **
    finalColor = pow(finalColor, vec3(1.0 / 2.2)); // Correction gamma
    FragColor = vec4(finalColor, alpha);
}
