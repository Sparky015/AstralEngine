

#ifndef BRDF_GLSL
#define BRDF_GLSL

#include "ColorTransforms.glsl"

const float PI = 3.14159265359;

struct Material
{
    vec3 BaseColor;
    float Roughness;
    float Metallic;
    vec3 Emission;
    vec3 Normal;
};


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


float CalcCascadeZFar(float zNear, float zFar, float cascadeNum, float totalCascades)
{
    const float blendFactor = .5;
    const float logComponent = blendFactor * (zNear * pow((zFar / zNear), cascadeNum / totalCascades));
    const float linearComponent = (1 - blendFactor) * (zNear + cascadeNum / totalCascades * (zFar - zNear));
    return logComponent + linearComponent;
}

int GetCascadeIndex(vec3 worldPosition)
{
    float distance = (u_SceneData.cameraView * vec4(worldPosition, 1.0f)).z * -1.0f;

    int cascadeNum = u_PushConstants.numShadowCascades - 1;
    for (int i = 0; i < u_PushConstants.numShadowCascades; i++)
    {
        if (distance < CalcCascadeZFar(u_PushConstants.cameraZNear, u_PushConstants.cameraZFar, i + 1, u_PushConstants.numShadowCascades)) { cascadeNum = i; break; }
    }
    return cascadeNum;
}

vec3 cascadeDebugOverlayColors[8] = { { 1.0, 0.0f, 0.0f }, { 0.0, 1.0f, 0.0f }, { 0.0, 0.0f, 1.0f }, { 0.5, 0.5f, 0.0f },
                                      { 0.0, 0.5f, 0.5f }, { 0.5, 0.0f, 0.5f }, { 1.0, 0.0f, 0.5f }, { 1.0, 5.0f, 0.0f } };


float CalculateShadowAtFrag(vec3 worldPosition, vec3 normal, vec3 lightVector)
{
    int cascadeNum = GetCascadeIndex(worldPosition);

    vec4 fragPositionLightSpace = u_LightMatrices.lightMatrices[cascadeNum] * vec4(worldPosition, 1.0f);
    vec3 projCoords = fragPositionLightSpace.xyz / fragPositionLightSpace.w;
    projCoords.xy = projCoords.xy * 0.5 + 0.5; // Transform into UV range for sampling shadow map


    float closestLightDepth = texture(u_DirectionalLightShadows, vec3(projCoords.xy, cascadeNum)).r;
    float currentCameraDepth = clamp(projCoords.z, 0, 1);

    float biasScalingFactor = u_PushConstants.shadowMapBias;
    float bias = max(biasScalingFactor * (1.0 - dot(normal, lightVector)), 0.0005);

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(u_DirectionalLightShadows, 0).rg;

    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            vec2 offset = vec2(float(x), float(y)) * texelSize;
            float pcfDepth = texture(u_DirectionalLightShadows, vec3(projCoords.xy + offset, cascadeNum)).r;
            shadow += currentCameraDepth - bias > pcfDepth ? 1.0 : 0.0; // 1 = Shadow, 0 = No Shadow
        }
    }
    shadow /= 9.0; // Average of 9 samples

    return shadow;
}


vec3 BRDF(Material material, vec3 worldPosition, vec3 viewVector)
{
    vec3 baseColor = material.BaseColor;
    float roughness = material.Roughness;
    float metallic = material.Metallic;
    vec3 emission = material.Emission;
    vec3 normal = material.Normal;


    baseColor = ConvertSRGBPimariesToAP1Primaries(baseColor);
    emission = ConvertSRGBPimariesToAP1Primaries(emission);


    vec3 finalLight = vec3(0.0f);
    for (int i = 0; i < u_SceneData.numLights; i++)
    {
        vec3 lightPosition = u_SceneLights.lights[i].lightPosition;
        vec3 lightColor = u_SceneLights.lights[i].lightColor;
        uint lightType = u_SceneLights.lights[i].lightType;

        lightColor = ConvertSRGBPimariesToAP1Primaries(lightColor);

        // Vectors
        normal = normalize(normal);

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
        baseReflectivity = mix(baseReflectivity, baseColor, metallic); // Mix based on metallic value
        vec3 specular = Fresnel(baseReflectivity, viewVector, halfwayVector);
        vec3 diffuse = (vec3(1.0) - specular) * (1.0f - metallic);

        vec3 lambert = baseColor / PI;

        vec3 cookTorranceNumerator = GGXNormalDistribution(alpha, normal, halfwayVector) * Shadowing(alpha, normal, viewVector, lightVector) * specular;
        float cookTorranceDenominator = 4.0 * max(dot(viewVector, normal), 0.0) * max(dot(lightVector, normal), 0.0);
        cookTorranceDenominator = max(cookTorranceDenominator, 0.000001);
        vec3 cookTorrance = cookTorranceNumerator / cookTorranceDenominator;

        vec3 BRDF = diffuse * lambert + cookTorrance;
        float shadow = CalculateShadowAtFrag(worldPosition, normal, lightVector);

        vec3 outgoingLight = BRDF * (1 - shadow) * lightColor * max(dot(lightVector, normal), 0.0) * lightAttenuation;

        // Cascaded Shadow Map Debug Overlay
        if (u_PushConstants.showCascadeDebugView == 1u)
        {
            outgoingLight = cascadeDebugOverlayColors[GetCascadeIndex(worldPosition)];
        }

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

    return finalLight;
}

#endif