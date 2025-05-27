#version 460

layout(location = 0) out vec4 color;

layout (location = 2) in vec2 a_TextureCoord;

layout (binding = 1) uniform sampler2D u_Texture;

void main()
{
    color = texture(u_Texture, a_TextureCoord);
}


