#version 330 core
layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
layout (location = 1) in vec2 aTextureCoord;
  
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 TextureCoord;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TextureCoord = aTextureCoord;
}       
