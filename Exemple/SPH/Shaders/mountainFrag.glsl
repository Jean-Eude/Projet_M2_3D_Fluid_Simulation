#version 460 core
out vec4 FragColor;

in vec3 pos;

void main()
{
    FragColor = vec4(vec3(pos), 1.);
}