#version 460 core

in vec4 couleur;
in float density;
in vec3 velocity;
in vec3 force;
in vec3 position;

out vec4 FragColor;

uniform float nearPlane;
uniform float farPlane;
uniform vec3 minAABB;
uniform vec3 maxAABB;

uniform vec2 screenSize; // Dimensions de l'écran

// Définition des paramètres du filtre bilatéral
const float spatialSigma = 1.0;  // Contrôle la pondération de la distance spatiale
const float depthSigma = 0.01;   // Contrôle la pondération des différences de profondeur

float LinearizeDepth(float depth) {
    float z = depth * 2.0 - 1.0; 
    return (2.0 * nearPlane * farPlane) / (farPlane + nearPlane - z * (farPlane - nearPlane));
}

float Gaussian(float x, float sigma) {
    return exp(-0.5 * (x * x) / (sigma * sigma));
}

void main() {
    vec2 fragCoord = gl_FragCoord.xy;
    vec2 pixelSize = 1.0 / screenSize;

    // Calcul de la profondeur linéaire courante
    float centerDepth = LinearizeDepth(gl_FragCoord.z);

    // Initialisation du filtre bilatéral
    float sum = 0.0;
    float totalWeight = 0.0;

    // Parcourir un voisinage (5x5 ou autre)
    for (int i = -2; i <= 2; i++) {
        for (int j = -2; j <= 2; j++) {
            // Coordonnées du voisin
            vec2 neighborCoord = fragCoord + vec2(i, j) * pixelSize;

            // Simuler la profondeur des voisins
            float neighborDepth = LinearizeDepth(gl_FragCoord.z);

            // Calcul des poids spatiaux et des différences de profondeur
            float spatialWeight = Gaussian(length(vec2(i, j)), spatialSigma);
            float depthWeight = Gaussian(centerDepth - neighborDepth, depthSigma);

            // Poids total
            float weight = spatialWeight * depthWeight;

            // Contribution au filtre bilatéral
            sum += neighborDepth * weight;
            totalWeight += weight;
        }
    }

    // Calcul de la profondeur lissée
    float smoothedDepth = sum / totalWeight;

    // Normalisation de la profondeur par rapport à la boîte englobante
    float boxDepthRange = length(maxAABB - minAABB);
    float normalizedDepth = smoothedDepth / boxDepthRange;

    // Limiter la valeur normalisée entre 0 et 1
    normalizedDepth = clamp(normalizedDepth, 0.0, 1.0);

    // Définir la couleur de sortie en fonction de la profondeur lissée
    FragColor = vec4(vec3(normalizedDepth), 1.0);
}
