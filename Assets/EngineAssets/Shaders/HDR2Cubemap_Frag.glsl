#version 460 core
out vec4 FragColor;

in vec3 localPos;

uniform sampler2D equirectangularMap;

const float PI = 3.14159265359;

// Fonction pour transformer une direction en coordonnées UV équirectangulaires
vec2 SampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= vec2(0.1591, 0.3183); // Constantes pour normaliser atan et asin
    uv += 0.5;
    return uv;
}

// Importance sampling de la sphère
vec3 IntegrateDiffuseIrradiance(vec3 normal)
{
    vec3 irradiance = vec3(0.0);
    const int sampleCount = 64; // Augmentez pour plus de précision
    for (int i = 0; i < sampleCount; ++i)
    {
        // Générer des directions uniformément distribuées autour de la normale
        vec2 xi = vec2(float(i) / sampleCount, float(i) / sampleCount); // Uniforme sur [0, 1]
        vec3 tangent = normalize(cross(normal, vec3(0.0, 1.0, 0.0)));
        vec3 bitangent = cross(normal, tangent);
        vec3 sampleVec = tangent * cos(2.0 * PI * xi.x) * sqrt(1.0 - xi.y) +
                         bitangent * sin(2.0 * PI * xi.x) * sqrt(1.0 - xi.y) +
                         normal * sqrt(xi.y);

        // Échantillonnage de la HDRI avec la direction générée
        vec2 uv = SampleSphericalMap(sampleVec);
        vec3 hdrColor = texture(equirectangularMap, uv).rgb;

        // Poids proportionnel au cosinus
        float NdotL = max(dot(normal, sampleVec), 0.0);
        irradiance += hdrColor * NdotL;
    }
    return irradiance * (PI / float(sampleCount));
}

void main()
{
    vec3 normal = normalize(localPos); // Normal locale (dirigeante)
    vec3 irradiance = IntegrateDiffuseIrradiance(normal);
    FragColor = vec4(irradiance, 1.0);
}
