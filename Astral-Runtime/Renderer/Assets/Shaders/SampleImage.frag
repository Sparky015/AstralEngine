#version 460

layout(location = 0) in vec3 v_WorldPosition;
layout(location = 1) in vec3 v_Normals;
layout(location = 2) in vec3 v_Tangents;
layout(location = 3) in vec3 v_Bitangents;
layout(location = 4) in vec2 v_TextureCoord;

layout (set = 1, binding = 0) uniform sampler2D u_Texture;

layout(location = 0) out vec4 color;

void main()
{
    vec4 texColor = texture(u_Texture, v_TextureCoord);
    if (texColor.a < 0.01f)
    {
        discard;
    }

    color = texColor;
}
