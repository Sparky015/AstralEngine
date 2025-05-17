#version 410

layout(location = 0) out vec4 color;

in vec4 v_Position;
in vec2 v_TextureCoord;

uniform sampler2D u_Texture;

void main()
{
    color = texture(u_Texture, v_TextureCoord);
}


