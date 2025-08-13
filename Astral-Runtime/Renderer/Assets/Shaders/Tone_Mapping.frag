#version 460

layout(location = 0) in vec3 v_WorldPosition;
layout(location = 1) in vec3 v_Normals;
layout(location = 2) in vec3 v_Tangents;
layout(location = 3) in vec3 v_Bitangents;
layout(location = 4) in vec2 v_TextureCoord;

layout (set = 1, binding = 0) uniform sampler2D u_SceneRender;
layout (set = 2, binding = 0) uniform sampler3D u_ToneMapLut; // LUT is ACES baked lut converting ACEScg to rec709 linear

layout(location = 0) out vec4 color;

void main()
{
    vec3 renderedColors = texture(u_SceneRender, v_TextureCoord).rgb;
    color = texture(u_ToneMapLut, renderedColors);
    color = vec4(renderedColors, 1.0f);
}
