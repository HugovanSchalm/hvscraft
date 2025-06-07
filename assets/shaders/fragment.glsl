#version 330 core

in vec2 TextureCoord;

uniform sampler2D Texture;

out vec4 FragColor;  

void main()
{
    FragColor = texture2D(Texture, TextureCoord);
}
