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


layout(set = 2, binding = 0) uniform sampler2D u_AlbedoInput;
layout(set = 2, binding = 1) uniform sampler2D u_MetallicInput;
layout(set = 2, binding = 2) uniform sampler2D u_RoughnessInput;
layout(set = 2, binding = 3) uniform sampler2D u_EmissionInput;
layout(set = 2, binding = 4) uniform sampler2D u_NormalInput;
layout(set = 2, binding = 5) uniform sampler2D u_DepthBufferInput;
layout(set = 2, binding = 6) uniform sampler2D u_DirectionalLightShadows;


layout(push_constant) uniform PushConstantData {
    mat4 lightSpaceMatrix;
} u_PushConstants;

layout(location = 0) out vec4 outColor;


#include "ColorUtilities.glsl"



const float PI = 3.14159265359;

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
    float denominator = PI * pow(pow(NdotH, 2.0f) * (pow(alpha, 2.0f) - 1.0f) + 1.0, 2.0f);
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


float CalculateShadowAtFrag(vec3 worldPosition, vec3 normal, vec3 lightVector)
{
    vec4 fragPositionLightSpace = u_PushConstants.lightSpaceMatrix * vec4(worldPosition, 1.0f);
    vec3 projCoords = fragPositionLightSpace.xyz / fragPositionLightSpace.w;
    projCoords.xy = projCoords.xy * 0.5 + 0.5; // Transform into UV range for sampling shadow map

    float closestDepth = texture(u_DirectionalLightShadows, projCoords.xy).r;
    float currentDepth = projCoords.z;

    float bias = max(0.05 * (1.0 - dot(normal, lightVector)), 0.005);

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(u_DirectionalLightShadows, 0);

    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            vec2 offset = vec2(float(x), float(y)) * texelSize;
            float pcfDepth = texture(u_DirectionalLightShadows, projCoords.xy + offset).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0; // Average of 9 samples

    return shadow;
}


void main()
{
    vec3 baseColor = texture(u_AlbedoInput, v_TextureCoord).rgb;
    baseColor = ConvertSRGBPimariesToAP1Primaries(baseColor);

    float metallic = texture(u_MetallicInput, v_TextureCoord).r;
    float roughness = texture(u_RoughnessInput, v_TextureCoord).r;

    vec3 emission = texture(u_EmissionInput, v_TextureCoord).rgb;
    emission = ConvertSRGBPimariesToAP1Primaries(emission);

    vec3 normal = texture(u_NormalInput, v_TextureCoord).rgb;
    normal = normal * 2.0 - 1.0;
    normal = normalize(normal);


    vec3 worldPosition = GetWorldPosition();
    vec3 cameraPosition = u_SceneData.cameraPosition;
    vec3 viewVector = normalize(cameraPosition - worldPosition);

    vec3 finalLight = vec3(0.0f);

    if (u_SceneData.numLights == 0)
    {
        outColor = vec4(emission, 1.0f);
    }

    for (int i = 0; i < u_SceneData.numLights; i++)
    {
        vec3 lightPosition = u_SceneLights.lights[i].lightPosition;
        vec3 lightColor = u_SceneLights.lights[i].lightColor;
        lightColor = ConvertSRGBPimariesToAP1Primaries(lightColor);

        uint lightType = u_SceneLights.lights[i].lightType;

        // Vectors
        vec3 lightVector;
        if (lightType == LIGHT_TYPE_POINT) { lightVector = normalize(lightPosition - worldPosition); }
        else if (lightType == LIGHT_TYPE_DIRECTIONAL) { lightVector = normalize(-lightPosition); } // Light position is direction for directional lights

        vec3 halfwayVector = normalize(viewVector + lightVector);
        float lightDistance = length(lightPosition - worldPosition);
        float lightAttenuation = 1.0 / (lightDistance * lightDistance); // quadratic attenuation formula

        if (lightType == LIGHT_TYPE_DIRECTIONAL) { lightAttenuation = 1; } // No attenuation for directional lights

            // PBR Equation in Full
        vec3 baseReflectivity = vec3(0.04);
        float alpha = pow(roughness, 2.0f);
        baseReflectivity = mix(baseReflectivity, baseColor, metallic.r); // Mix based on metallic value
        vec3 specular = Fresnel(baseReflectivity, viewVector, halfwayVector);
        vec3 diffuse = (vec3(1.0) - specular) * (1.0f - metallic.r);

        vec3 lambert = baseColor / PI;

        vec3 cookTorranceNumerator = GGXNormalDistribution(alpha, normal, halfwayVector) * Shadowing(alpha, normal, viewVector, lightVector) * specular;
        float cookTorranceDenominator = 4.0 * max(dot(viewVector, normal), 0.0) * max(dot(lightVector, normal), 0.0);
        cookTorranceDenominator = max(cookTorranceDenominator, 0.000001);
        vec3 cookTorrance = cookTorranceNumerator / cookTorranceDenominator;

        vec3 BRDF = diffuse * lambert + cookTorrance;
        float shadow = CalculateShadowAtFrag(worldPosition, normal, lightVector);

        vec3 outgoingLight = BRDF * (1 - shadow) * lightColor * max(dot(lightVector, normal), 0.0) * lightAttenuation;
        finalLight += outgoingLight;
    }


    vec3 F0 = mix(vec3(0.04), baseColor, metallic);
    vec3 F = IBLFresnelSchlickRoughness(max(dot(normal, viewVector), 0.0), F0, roughness);

    // Indirect Specular
    vec3 reflectionVector = reflect(-viewVector, normal);
    float totalMips = textureQueryLevels(u_PrefilteredEnvironment) - 1;
    vec3 prefilteredColor = textureLod(u_PrefilteredEnvironment, reflectionVector,  roughness * totalMips).rgb;
    prefilteredColor = ConvertSRGBPimariesToAP1Primaries(prefilteredColor);

    vec2 viewAngleRoughnessInput = vec2(max(dot(normal, viewVector), 0.0), roughness);
    viewAngleRoughnessInput.r -= .01; // This avoids head on specular lighting for environment lighting which caused issues. This is a fix for now.
    clamp(viewAngleRoughnessInput.r, 0.0f, 1.0f);

    vec2 envBRDF  = texture(u_BRDFLut, viewAngleRoughnessInput).rg;
    vec3 specular = prefilteredColor * (F * envBRDF.x + envBRDF.y);

    // Indirect Diffuse
    vec3 kS = F;
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - metallic;

    vec3 irradiance = texture(u_Irradiance, normal).rgb;
    irradiance = ConvertSRGBPimariesToAP1Primaries(irradiance);

    vec3 diffuse = irradiance * baseColor;

    vec3 ambient = u_SceneData.ambientLightConstant * (kD * diffuse + specular);
    finalLight += ambient + emission;

    outColor = vec4(finalLight, 1.0f);
}
