#shader vertex

#version 410

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec4 v_Position;
out vec2 v_TexCoord;

void main()
{
    v_Position = position;
    v_TexCoord = texCoord;
    gl_Position = position;
}


