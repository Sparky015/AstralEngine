#version 460

layout (location = 0) in vec2 v_Position;
layout (location = 1) out vec2 v_TexCoord;

vec2 UVs[3] = {{0.0f, 0.0f}, {1.0f, 1.0f}, {1.0f, 0.0f}};

void main()
{
    v_TexCoord = UVs[gl_VertexIndex];
    gl_Position = vec4(v_Position, 0.0, 1.0);
}