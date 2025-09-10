#version 460

layout(location = 0) in vec3 v_Position;
layout(location = 1) in vec3 v_Normals;
layout(location = 2) in vec3 v_Tangents;
layout(location = 3) in vec3 v_Bitangents;

layout (set = 1, binding = 0) uniform samplerCube u_Cubemap;

layout(push_constant) uniform PushConstant {
    float environmentMapBlur;
} u_PushConstant;

layout(location = 0) out vec4 color;



vec3 ConvertSRGBPimariesToAP1Primaries(vec3 srgbPrimaries)
{
    const mat3 srgbPrimariesToAP1Primaries = mat3( 0.613132422390542, 0.070124380833917, 0.020587657528185,
                                                   0.339538015799666, 0.916394011313573, 0.109574571610682,
                                                   0.047416696048269, 0.013451523958235, 0.869785404035327 );
    return srgbPrimariesToAP1Primaries * srgbPrimaries;
}

void main()
{
    float totalMips = textureQueryLevels(u_Cubemap) - 1;
    float lodLevel = clamp(u_PushConstant.environmentMapBlur, 0, 1) * totalMips;

    vec3 srgbPrimaries = textureLod(u_Cubemap, v_Position, lodLevel).rgb;
    vec3 ap1Primaries = ConvertSRGBPimariesToAP1Primaries(srgbPrimaries);
    color = vec4(ap1Primaries, 1.0f);
}