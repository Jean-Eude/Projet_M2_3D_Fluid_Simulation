#version 460 core

out vec4 FragColor;

in vec3 Pos; 
in vec2 Tex;         
in vec3 Normal; 

uniform vec3 lightPositions; // Positions des lumières (ici une seule)
uniform vec3 camPos;            // Position de la caméra

void main()
{           
    // Couleur de base de l'objet
    vec3 objectColor = vec3(1.0, 0.0, 0.0); // Rouge

    // Couleur de la lumière
    vec3 lightColor = vec3(1.0); // Lumière blanche

    // Normalisation de la normale
    vec3 norm = normalize(Normal);

    // Calcul de la direction de la lumière
    vec3 lightDir = normalize(lightPositions - Pos);

    // Composante diffuse
    float diff = max(dot(lightDir, norm), 0.0);
    vec3 diffuse = diff * lightColor;

    // Composante spéculaire uniquement si la face est éclairée
    vec3 reflectDir = reflect(-lightDir, norm);
    vec3 viewDir = normalize(camPos - Pos);
    float spec = diff > 0.0 ? pow(max(dot(viewDir, reflectDir), 0.0), 32.0) : 0.0;
    vec3 specular = spec * lightColor;

    // Résultat final
    vec3 ambient = 0.1 * objectColor;
    vec3 result = ambient * objectColor + diffuse * objectColor + specular;

    FragColor = vec4(result, 1.0);
}
