#version 460 core
out vec4 FragColor;

uniform sampler2D tex0;
uniform sampler2D tex1;
uniform sampler2D tex2;

in vec2 TexCoord;

void main()
{
    FragColor = texture(tex0, TexCoord);
}