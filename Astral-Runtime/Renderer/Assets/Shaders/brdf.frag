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

struct Light {
    vec3 lightPosition;
    vec3 lightColor;
};

layout (set = 0, binding = 1) readonly buffer Lights {
    Light[] lights;
} u_SceneLights;

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


void main()
{
    vec3 baseColor = texture(u_BaseColor, v_TextureCoord).rgb;
    vec3 metallic = texture(u_Metallic, v_TextureCoord).rgb;
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

        if (u_ModelData.hasDirectXNormals != 0)
        {
            normal.g *= -1.0f;
        }
    }

    vec3 cameraPosition = u_SceneData.cameraPosition;
    vec3 finalLight = vec3(0.0f);

    if (u_SceneData.numLights == 0)
    {
        color = vec4(emission, 1.0f);
    }

    for (int i = 0; i < u_SceneData.numLights; i++)
    {
        vec3 lightPosition = u_SceneLights.lights[i].lightPosition;
        vec3 lightColor = u_SceneLights.lights[i].lightColor;

        // Vectors
        normal = normalize(normal);
        vec3 viewVector = normalize(cameraPosition - v_WorldPosition);
        vec3 lightVector = normalize(lightPosition - v_WorldPosition);
        vec3 halfwayVector = normalize(viewVector + lightVector);
        float lightDistance = length(lightPosition - v_WorldPosition);
        float lightAttenuation = 1.0 / (lightDistance * lightDistance); // quadratic attenuation formula



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
        vec3 outgoingLight = BRDF * lightColor * max(dot(lightVector, normal), 0.0) * lightAttenuation;
        finalLight += outgoingLight;
    }

    vec3 ambient = u_SceneData.ambientLightConstant * baseColor * (1.0 - metallic.r);
    finalLight += ambient + emission;


    color = vec4(finalLight, 1.0f);
}
