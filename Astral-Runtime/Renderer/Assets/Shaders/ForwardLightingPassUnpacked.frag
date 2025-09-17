#version 460

layout(location = 0) in vec3 v_WorldPosition;
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

layout(set = 2, binding = 0) uniform sampler2DArray u_DirectionalLightShadows;

layout (set = 3, binding = 0) uniform LightMatrices {
    mat4 lightMatrices[8];
} u_LightMatrices;

layout (set = 4, binding = 0) uniform sampler2D u_BaseColor;
layout (set = 4, binding = 1) uniform sampler2D u_Metallic;
layout (set = 4, binding = 2) uniform sampler2D u_Roughness;
layout (set = 4, binding = 3) uniform sampler2D u_Emission;
layout (set = 4, binding = 4) uniform sampler2D u_Normals;

layout (push_constant) uniform ModelData {
    mat4 transform;
    uint hasNormalMap;
    uint hasDirectXNormals;
    float cameraZNear;
    float cameraZFar;
    int numShadowCascades;
    uint showCascadeDebugView;
} u_PushConstants;

layout(location = 0) out vec4 color;


#include "BRDF.glsl"
#include "Utilities.glsl"


void main()
{
    Material material;
    material.BaseColor = texture(u_BaseColor, v_TextureCoord).rgb;
    material.Roughness = texture(u_Roughness, v_TextureCoord).r;
    material.Metallic = texture(u_Metallic, v_TextureCoord).r;
    material.Emission = texture(u_Emission, v_TextureCoord).rgb;

    vec3 normal = v_Normals;

    if (u_PushConstants.hasNormalMap != 0)
    {
        vec3 tangentSpaceNormal = texture(u_Normals, v_TextureCoord).rgb;
        normal = CalculateNormalFromMap(tangentSpaceNormal, v_Normals, v_Tangents, v_Bitangents, u_PushConstants.hasDirectXNormals);
    }

    material.Normal = normal;

    vec3 worldPosition = v_WorldPosition;
    vec3 viewVector = normalize(u_SceneData.cameraPosition - worldPosition);

    vec3 finalColor = BRDF(material, worldPosition, viewVector);
    color = vec4(finalColor, 1.0f);
}
