#version 460

layout (location = 0) in vec2 a_TextureCoord;

layout (binding = 0) uniform sampler2D u_Texture;

layout(location = 0) out vec4 color;

void main()
{
    color = texture(u_Texture, a_TextureCoord);
}
