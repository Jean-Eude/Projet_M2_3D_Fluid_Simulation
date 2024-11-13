#version 460 core
out vec4 FragColor;

uniform sampler2D tex;
uniform sampler2D tex1;
uniform sampler2D tex2;
uniform sampler2D tex3;

in vec2 TexCoord;

void main()
{
    FragColor = texture(tex2, TexCoord);
}