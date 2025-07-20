#version 460

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normals;
layout(location = 2) in vec3 a_Tangents;
layout(location = 3) in vec3 a_Bitangents;

layout(location = 0) out vec3 v_Position;
layout(location = 1) out vec3 v_Normals;
layout(location = 2) out vec3 v_Tangents;
layout(location = 3) out vec3 v_Bitangents;

void main()
{
    v_Position = a_Position;
    v_Normals = a_Normals;
    v_Tangents = a_Tangents;
    v_Bitangents = a_Bitangents;
    v_TextureCoord = a_TextureCoord;

    gl_Position = vec4(a_Position, 1.0f);
}
