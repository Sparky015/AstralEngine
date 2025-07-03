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
} u_ModelData;

layout(location = 0) out vec3 albedoProp;
layout(location = 1) out float metallicProp;
layout(location = 2) out float roughnessProp;
layout(location = 3) out vec3 emissionProp;
layout(location = 4) out vec3 normalProp;


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
        tangentSpaceNormal = normalize(tangentSpaceNormal * 2.0 - 1.0); // Convert from [0, 1] to [-1, 1] and normalize
        vec3 N = normalize(v_Normals);
        vec3 T = normalize(v_Tangents);
        vec3 B = normalize(v_Bitangents);
        T = normalize(T - N * dot(N, T));
        B = cross(N, T);
        mat3 TBN = mat3(T, B, N);
        normal = normalize(TBN * tangentSpaceNormal);
    }

    albedoProp = baseColor;
    metallicProp = metallic;
    roughnessProp = roughness;
    emissionProp = emission;
    normalProp = normal;
}
