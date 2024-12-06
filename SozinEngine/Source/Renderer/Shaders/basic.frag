#shader fragment

#version 410

layout(location = 0) out vec4 color;

in vec4 v_Position;
in vec2 v_TexCoord;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
    color = texture(u_Texture, v_TexCoord) * u_Color;
}


