#version 460
#extension GL_ARB_shader_viewport_layer_array : require

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normals;
layout(location = 2) in vec3 a_Tangents;
layout(location = 3) in vec3 a_Bitangents;
layout(location = 4) in vec2 a_TextureCoord;

layout(push_constant) uniform PushConstantData {
    mat4 modelMatrix;
} u_PushConstants;

layout (set = 1, binding = 0) uniform LightMatrices {
    mat4 lightMatrices[8];
} u_LightMatrices;

void main()
{
    gl_Position = u_LightMatrices.lightMatrices[gl_InstanceIndex] * u_PushConstants.modelMatrix * vec4(a_Position, 1.0);
    gl_Layer = gl_InstanceIndex;
}
