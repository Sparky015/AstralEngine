#version 460

layout(location = 0) in vec3 a_Position;

layout(push_constant) uniform PushConstantData {
    mat4 lightSpaceMatrix;
    mat4 modelMatrix;
} u_PushConstants;

void main()
{
    gl_Position = u_PushConstants.lightSpaceMatrix * u_PushConstants.modelMatrix * vec4(a_Position, 1.0);
}
