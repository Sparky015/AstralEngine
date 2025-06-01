#version 460

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TextureCoord;

layout (push_constant) uniform Transform { mat4 transform; } u_Projection;

layout (location = 0) out vec2 v_TextureCoord;

void main()
{
    v_TextureCoord = a_TextureCoord;
    gl_Position = u_Projection.transform * vec4(a_Position, 1.0f);
}
