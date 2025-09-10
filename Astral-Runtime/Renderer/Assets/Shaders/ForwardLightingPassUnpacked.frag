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

layout(set = 2, binding = 0) uniform sampler2D u_DirectionalLightShadows;

layout (set = 3, binding = 0) uniform LightMatrices {
    mat4 lightMatrices;
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
} u_ModelData;

layout(location = 0) out vec4 color;


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

float CalculateShadowAtFrag(vec3 worldPosition, vec3 normal, vec3 lightVector)
{
    vec4 fragPositionLightSpace = u_LightMatrices.lightMatrices * vec4(worldPosition, 1.0f);
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

vec3 ConvertSRGBPimariesToAP1Primaries(vec3 srgbPrimaries)
{
    const mat3 srgbPrimariesToAP1Primaries = mat3( 0.613132422390542, 0.070124380833917, 0.020587657528185,
    0.339538015799666, 0.916394011313573, 0.109574571610682,
    0.047416696048269, 0.013451523958235, 0.869785404035327 );
    return srgbPrimariesToAP1Primaries * srgbPrimaries;
}

void main()
{
    vec3 baseColor = texture(u_BaseColor, v_TextureCoord).rgb;
    baseColor = ConvertSRGBPimariesToAP1Primaries(baseColor);

    vec3 metallic = texture(u_Metallic, v_TextureCoord).rgb;
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

        vec3 N = normalize(v_Normals);
        vec3 T = normalize(v_Tangents);
        vec3 B = normalize(v_Bitangents);
        T = normalize(T - N * dot(N, T));
        B = cross(N, T);
        mat3 TBN = mat3(T, B, N);
        normal = normalize(TBN * tangentSpaceNormal);

        if (u_ModelData.hasDirectXNormals != 0)
        {
            normal = normal * -1;
        }
    }

    vec3 worldPosition = v_WorldPosition;
    vec3 cameraPosition = u_SceneData.cameraPosition;
    vec3 viewVector = normalize(cameraPosition - worldPosition);

    vec3 finalLight = vec3(0.0f);

    if (u_SceneData.numLights == 0)
    {
        color = vec4(emission, 1.0f);
    }

    for (int i = 0; i < u_SceneData.numLights; i++)
    {
        vec3 lightPosition = u_SceneLights.lights[i].lightPosition;
        vec3 lightColor = u_SceneLights.lights[i].lightColor;
        uint lightType = u_SceneLights.lights[i].lightType;

        // Vectors
        normal = normalize(normal);
        vec3 viewVector = normalize(cameraPosition - v_WorldPosition);

        vec3 lightVector;
        if (lightType == LIGHT_TYPE_POINT) { lightVector = normalize(lightPosition - worldPosition); }
        else if (lightType == LIGHT_TYPE_DIRECTIONAL) { lightVector = normalize(-lightPosition); } // Light position is direction for directional lights

        vec3 halfwayVector = normalize(viewVector + lightVector);
        float lightDistance = length(lightPosition - v_WorldPosition);
        float lightAttenuation = 1.0 / (lightDistance * lightDistance); // quadratic attenuation formula

        if (lightType == LIGHT_TYPE_DIRECTIONAL) { lightAttenuation = 1; } // No attenuation for directional lights


        // PBR Equation in Full
        vec3 baseReflectivity = vec3(0.04);
        float alpha = pow(roughness, 2.0f);
        baseReflectivity = mix(baseReflectivity, baseColor, metallic.r); // Mix based on metallic value
        vec3 specular = Fresnel(baseReflectivity, viewVector, halfwayVector);
        vec3 diffuse = (vec3(1.0) - specular) * (1.0f - metallic.r);

        vec3 lambert = baseColor / 3.1415;

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
    vec3 diffuse = irradiance * baseColor;

    vec3 ambient = u_SceneData.ambientLightConstant * (kD * diffuse + specular);
    finalLight += ambient + emission;

    color = vec4(finalLight, 1.0f);
}
