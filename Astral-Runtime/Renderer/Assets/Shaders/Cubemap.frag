#version 460

layout(location = 0) in vec3 v_Position;
layout(location = 1) in vec3 v_Normals;
layout(location = 2) in vec3 v_Tangents;
layout(location = 3) in vec3 v_Bitangents;

layout (set = 1, binding = 0) uniform samplerCube u_Cubemap;


layout(location = 0) out vec4 color;

void main()
{
    color = texture(u_Cubemap, v_Position);
}