#version 330 core

in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

void main() {
    vec3 lightPos = vec3(10.0, 10.0, 10.0);
    vec3 lightColor = vec3(1.0, 1.0, 1.0);
    vec3 objectColor = vec3(0.0, 1.0, 1.0);

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    
    float diff = max(dot(norm, lightDir), 0.3);
        
    FragColor = vec4(diff * lightColor * objectColor, 1.0);
}
