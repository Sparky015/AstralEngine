#version 460

layout(location = 0) in vec3 v_QuadFragPosition;
layout(location = 1) in vec3 v_Normals;
layout(location = 2) in vec3 v_Tangents;
layout(location = 3) in vec3 v_Bitangents;
layout(location = 4) in vec2 v_TextureCoord;

layout (set = 0, binding = 0) uniform SceneData {
    mat4 cameraViewProjection;
    mat4 cameraView;
    mat4 cameraProjection;
    mat4 inverseCameraView;
    mat4 inverseCameraProjection;
    vec2 screenSize;
    vec3 cameraPosition;
    uint numLights;
    float ambientLightConstant;
    uint numShadowCascades;
} u_SceneData;

const uint LIGHT_TYPE_POINT = 0;
const uint LIGHT_TYPE_DIRECTIONAL = 1;

struct Light {
    vec3 lightPosition;
    vec3 lightColor;
    uint lightType;
};

layout (set = 0, binding = 1) readonly buffer Lights {
    Light[] lights;
} u_SceneLights;

layout (set = 1, binding = 0) uniform samplerCube u_PrefilteredEnvironment;
layout (set = 1, binding = 1) uniform samplerCube u_Irradiance;
layout (set = 1, binding = 2) uniform sampler2D u_BRDFLut;

layout (set = 2, binding = 0) uniform LightMatrices {
    mat4 lightMatrices[8];
} u_LightMatrices;

layout(set = 3, binding = 0) uniform sampler2D u_AlbedoInput;
layout(set = 3, binding = 1) uniform sampler2D u_MetallicInput;
layout(set = 3, binding = 2) uniform sampler2D u_RoughnessInput;
layout(set = 3, binding = 3) uniform sampler2D u_EmissionInput;
layout(set = 3, binding = 4) uniform sampler2D u_NormalInput;
layout(set = 3, binding = 5) uniform sampler2D u_DepthBufferInput;
layout(set = 3, binding = 6) uniform sampler2DArray u_DirectionalLightShadows;


layout(location = 0) out vec4 outColor;

// Find world position of frag from depth buffer
vec3 GetWorldPosition()
{
    vec3 worldPosition;

    float depth = texture(u_DepthBufferInput, v_TextureCoord).r;

    vec4 clipSpacePosition;
    clipSpacePosition.x = (v_TextureCoord.x) * 2.0 - 1.0;
    clipSpacePosition.y = (v_TextureCoord.y) * 2.0 - 1.0;
    clipSpacePosition.z = depth;
    clipSpacePosition.w = 1.0;

    vec4 viewSpacePosition = u_SceneData.inverseCameraProjection * clipSpacePosition;
    viewSpacePosition /= viewSpacePosition.w;

    vec4 worldPosHomogeneous = u_SceneData.inverseCameraView * viewSpacePosition;
    worldPosition = worldPosHomogeneous.xyz;// / worldPosHomogeneous.w;

    return worldPosition;
}

// GGX/Trowbridge-Reitz Normal Distribution Function
float GGXNormalDistribution(float alpha, vec3 N, vec3 H)
{
    float numerator = pow(alpha, 2.0f);

    float NdotH = max(dot(N, H), 0.0f);
    float denominator = 3.1415 * pow(pow(NdotH, 2.0f) * (pow(alpha, 2.0f) - 1.0f) + 1.0, 2.0f);
    denominator = max(denominator, 0.00001f);

    return numerator / denominator;
}

// Schlick-Beckmann Geometry Shadowing Function
float G1(float alpha, vec3 N, vec3 X)
{
    float numerator = max(dot(N, X), 0.0);

    float k = alpha / 2.0;
    float denominator = max(dot(N, X), 0.0) * (1.0 - k) + k;
    denominator = max(denominator, 0.000001);

    return numerator / denominator;
}

// Smith Model
float Shadowing(float alpha, vec3 N, vec3 V, vec3 L)
{
    return G1(alpha, N, V) * G1(alpha, N, L);
}

// Fresnel-Schlick Function
vec3 Fresnel(vec3 F0, vec3 V, vec3 H)
{
    return F0 + (vec3(1.0) - F0) * pow(1 - max(dot(V, H), 0.0), 5.0);
}

vec3 IBLFresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}

float frustumRanges[3] = {20.0f, 100.0f, 1000.0f};

float CalculateShadowAtFrag(vec3 worldPosition, vec3 normal, vec3 lightVector)
{
    float distance = (u_SceneData.cameraView * vec4(worldPosition, 1.0f)).z * -1.0f;

    int cascadeNum = 2;
    for (int i = 0; i < 3; i++)
    {
        if (distance < frustumRanges[i]) { cascadeNum = i; break; }
    }

    vec4 fragPositionLightSpace = u_LightMatrices.lightMatrices[cascadeNum] * vec4(worldPosition, 1.0f);
    vec3 projCoords = fragPositionLightSpace.xyz / fragPositionLightSpace.w;
    projCoords.xy = projCoords.xy * 0.5 + 0.5; // Transform into UV range for sampling shadow map

#include "BRDF.glsl"
#include "Utilities.glsl"


    float closestDepth = texture(u_DirectionalLightShadows, vec3(projCoords.xy, cascadeNum)).r;
    float currentDepth = projCoords.z;

    float bias = max(0.05 * (1.0 - dot(normal, lightVector)), 0.005);

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(u_DirectionalLightShadows, 0).rg;

    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            vec2 offset = vec2(float(x), float(y)) * texelSize;
            float pcfDepth = texture(u_DirectionalLightShadows, vec3(projCoords.xy + offset, cascadeNum)).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0; // Average of 9 samples

    return shadow;
}


void main()
{
    Material material;
    material.BaseColor = texture(u_AlbedoInput, v_TextureCoord).rgb;
    material.Roughness = texture(u_RoughnessInput, v_TextureCoord).r;
    material.Metallic = texture(u_MetallicInput, v_TextureCoord).r;
    material.Emission = texture(u_EmissionInput, v_TextureCoord).rgb;

    vec3 normal = texture(u_NormalInput, v_TextureCoord).rgb;
    normal = normal * 2.0 - 1.0;
    material.Normal = normalize(normal);

    float depth = texture(u_DepthBufferInput, v_TextureCoord).r;
    vec3 worldPosition = GetWorldPosition(depth, v_TextureCoord, u_SceneData.inverseCameraProjection, u_SceneData.inverseCameraView);
    vec3 viewVector = normalize(u_SceneData.cameraPosition - worldPosition);

    vec3 finalColor = BRDF(material, worldPosition, viewVector);
    outColor = vec4(finalColor, 1.0f);
}
