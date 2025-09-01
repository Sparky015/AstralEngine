#version 460

layout(location = 0) in vec3 v_Position;
layout(location = 1) in vec3 v_Normals;
layout(location = 2) in vec3 v_Tangents;
layout(location = 3) in vec3 v_Bitangents;

layout (set = 1, binding = 0) uniform samplerCube u_Cubemap;

layout(push_constant) uniform PushConstant {
    float environmentMapBlur;
} u_PushConstant;

layout(location = 0) out vec4 color;

void main()
{
    float totalMips = textureQueryLevels(u_Cubemap) - 1;
    float lodLevel = clamp(u_PushConstant.environmentMapBlur, 0, 1) * totalMips;
    color = textureLod(u_Cubemap, v_Position, lodLevel);
}