#version 460

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normals;
layout(location = 2) in vec3 a_Tangents;
layout(location = 3) in vec3 a_Bitangents;
layout(location = 4) in vec2 a_TextureCoord;

layout(push_constant) uniform PushConstantData {
    mat4 lightSpaceMatrix;
    mat4 modelMatrix;
} u_PushConstants;

void main()
{
    gl_Position = u_PushConstants.lightSpaceMatrix * u_PushConstants.modelMatrix * vec4(a_Position, 1.0);
}
