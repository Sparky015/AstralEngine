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

layout (push_constant) uniform PushConstants {
    float cameraZNear;
    float cameraZFar;
    int numShadowCascades;
    uint showCascadeDebugView;
    float shadowMapBias;
    float shadowMapCascadeLogFactor;
} u_PushConstants;


layout(location = 0) out vec4 outColor;

#include "BRDF.glsl"
#include "Utilities.glsl"


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
