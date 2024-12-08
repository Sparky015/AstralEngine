#shader vertex

#version 410

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TextureCoord;

out vec4 v_Position;
out vec2 v_TextureCoord;

uniform mat4 u_Transform;

void main()
{

    v_Position = vec4(a_Position, 1.0f);
    v_TextureCoord = a_TextureCoord;
    gl_Position = u_Transform * vec4(a_Position, 1.0f);
}


