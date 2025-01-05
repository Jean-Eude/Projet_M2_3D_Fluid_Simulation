#version 460 core

out vec4 FragColor;

in vec3 Pos; 
in vec2 Tex;         
in vec3 Normal; 

uniform vec3 lightPositions;
uniform vec3 camPos; 

uniform vec3 ColorLum;

uniform vec3 Albedo;                        // Couleur
uniform float Metallic;                 // Métallique
uniform float Roughness;                // Rugosité
uniform float AO;                       // Occlusion ambiante

const float PI = 3.14159265359;


// ----------------------------------------------------------------------------
// Distribution GGX
float DistributionGGX(vec3 N, vec3 H, float Roughness)
{
    float a = Roughness * Roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float nom = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

// Geometry Schlick GGX
float GeometrySchlickGGX(float NdotV, float Roughness)
{
    float r = (Roughness + 1.0);
    float k = (r * r) / 8.0;

    float nom = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

// Geometry Smith
float GeometrySmith(vec3 N, vec3 V, vec3 L, float Roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, Roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, Roughness);

    return ggx1 * ggx2;
}

// Fresnel Schlick
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}
// ----------------------------------------------------------------------------

void main() {
    vec3 N = normalize(Normal);                 // Normal to surface
    vec3 V = normalize(camPos - Pos);           // Vector to camera
    vec3 L = normalize(lightPositions - (Pos + 0.01 * N)); // Offset light position
    vec3 H = normalize(V + L);                  // Halfway vector

    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, Albedo, Metallic);

    // Specular BRDF terms
    float NdotL = max(dot(N, L), 0.001);        // Clamp NdotL
    float NdotV = max(dot(N, V), 0.001);        // Clamp NdotV
    float a = max(0.01, Roughness * Roughness); // Prevent very small roughness
    float NDF = DistributionGGX(N, H, Roughness);
    float G = GeometrySmith(N, V, L, Roughness);
    vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

    // Final specular contribution
    float denominator = max(4.0 * NdotV * NdotL, 0.001);
    vec3 specular = clamp((NDF * G * F) / denominator, 0.0, 1.0);

    // Diffuse term
    vec3 kD = (1.0 - Metallic) * (1.0 - F); // Diffuse energy
    vec3 diffuse = kD * Albedo / PI;

    // Light contribution
    vec3 Lo = (diffuse + specular) * ColorLum * NdotL;

    // Ambient term with clamped AO
    vec3 ambient = vec3(0.03) * Albedo * max(AO, 0.1);

    // Combine contributions
    vec3 color = ambient + Lo;

    // Gamma correction
    color = pow(color, vec3(1.0 / 2.2));

    FragColor = vec4(color, 1.0);
}