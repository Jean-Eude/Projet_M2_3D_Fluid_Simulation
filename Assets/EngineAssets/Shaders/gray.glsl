#version 460 core
out vec4 FragColor;

uniform sampler2D tex0;
uniform sampler2D tex1;
uniform sampler2D tex2;
uniform sampler2D tex3;
uniform sampler2D tex4;
uniform sampler2D tex5;

in vec2 TexCoord;

void main()
{
    FragColor = texture(tex1, TexCoord);
}