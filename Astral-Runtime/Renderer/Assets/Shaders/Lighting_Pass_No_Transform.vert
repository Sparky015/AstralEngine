#version 460

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normals;
layout(location = 2) in vec3 a_Tangents;
layout(location = 3) in vec3 a_Bitangents;
layout(location = 4) in vec2 a_TextureCoord;

void main()
{
    gl_Position = vec4(a_Position * 2, 1.0f);
}
