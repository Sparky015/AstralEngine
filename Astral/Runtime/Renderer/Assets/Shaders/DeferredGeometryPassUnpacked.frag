#version 460

layout(location = 0) in vec3 v_WorldPosition;
layout(location = 1) in vec3 v_Normals;
layout(location = 2) in vec3 v_Tangents;
layout(location = 3) in vec3 v_Bitangents;
layout(location = 4) in vec2 v_TextureCoord;

layout (set = 1, binding = 0) uniform sampler2D u_BaseColor;
layout (set = 1, binding = 1) uniform sampler2D u_Metallic;
layout (set = 1, binding = 2) uniform sampler2D u_Roughness;
layout (set = 1, binding = 3) uniform sampler2D u_Emission;
layout (set = 1, binding = 4) uniform sampler2D u_Normals;

layout (push_constant) uniform ModelData {
    mat4 transform;
    uint hasNormalMap;
    uint hasDirectXNormals;
} u_ModelData;

layout(location = 0) out vec4 albedoProp;
layout(location = 1) out vec4 metallicProp;
layout(location = 2) out vec4 roughnessProp;
layout(location = 3) out vec4 emissionProp;
layout(location = 4) out vec4 normalProp;

#include "Utilities.glsl"

void main()
{
    vec3 baseColor = texture(u_BaseColor, v_TextureCoord).rgb;
    float metallic = texture(u_Metallic, v_TextureCoord).r;
    float roughness = texture(u_Roughness, v_TextureCoord).r;
    vec3 emission = texture(u_Emission, v_TextureCoord).rgb;
    vec3 tangentSpaceNormal = texture(u_Normals, v_TextureCoord).rgb;

    vec3 normal = v_Normals;

    if (u_ModelData.hasNormalMap != 0)
    {
        tangentSpaceNormal = tangentSpaceNormal * 2.0 - 1.0;

        if (u_ModelData.hasDirectXNormals != 0)
        {
            tangentSpaceNormal.g = tangentSpaceNormal.g * -1.0f;
        }
        tangentSpaceNormal = normalize(tangentSpaceNormal);

        mat3 TBN = CalculateTBNMatrix(v_Normals, v_Tangents, v_Bitangents);
        normal = normalize(TBN * tangentSpaceNormal);

        if (u_ModelData.hasDirectXNormals != 0)
        {
            normal = normal * -1;
        }
    }

    albedoProp = vec4(baseColor, 1.0f);
    metallicProp = vec4(metallic, 0.0f, 0.0f, 1.0f);
    roughnessProp = vec4(roughness, 0.0f, 0.0f, 1.0f);
    emissionProp = vec4(emission, 1.0f);
    normalProp = vec4((normal + 1.0f) * .5f, 1.0f);
}
