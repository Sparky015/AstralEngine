#version 460

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normals;
layout(location = 2) in vec2 a_TextureCoord;

layout (set = 0, binding = 0) uniform ViewProjection { mat4 viewProjectionMat; } u_ViewProjection;
layout (push_constant) uniform ModelTransform { mat4 transform; } u_ModelProjection;

layout (location = 0) out vec2 v_TextureCoord;

void main()
{
    v_TextureCoord = a_TextureCoord;
    gl_Position = u_ViewProjection.viewProjectionMat * u_ModelProjection.transform * vec4(a_Position, 1.0f);
}
