#version 460

layout(location = 0) in vec3 v_Position;
layout(location = 1) in vec3 v_Normals;
layout(location = 2) in vec3 v_Tangents;
layout(location = 3) in vec3 v_Bitangents;


layout(set = 1, binding = 0) uniform Data {
    float roughness;
} u_Data;
layout(set = 1, binding = 1) uniform samplerCube u_EnvironmentMap;


layout(location = 0) out vec4 color;

const float PI = 3.14159265359;


// Helper function to get low discrepancy samples
float RadicalInverse_VdC(uint bits)
{
    bits = (bits << 16u) | (bits >> 16u);
    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
    return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}

// Function to get low discrepancy samples
vec2 Hammersley(uint i, uint N)
{
    return vec2(float(i)/float(N), RadicalInverse_VdC(i));
}


vec3 ImportanceSampleGGX(vec2 Xi, vec3 N, float roughness)
{
    float a = roughness * roughness;

    float phi = 2.0 * PI * Xi.x;
    float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a*a - 1.0) * Xi.y));
    float sinTheta = sqrt(1.0 - cosTheta*cosTheta);

    // from spherical coordinates to cartesian coordinates
    vec3 H;
    H.x = cos(phi) * sinTheta;
    H.y = sin(phi) * sinTheta;
    H.z = cosTheta;

    // from tangent-space vector to world-space sample vector
    vec3 up        = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
    vec3 tangent   = normalize(cross(up, N));
    vec3 bitangent = cross(N, tangent);

    vec3 sampleVec = tangent * H.x + bitangent * H.y + N * H.z;
    return normalize(sampleVec);
}

float NdfGGX_Pdf(float NdotH, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH2 = NdotH * NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}


void main()
{
    vec3 N = normalize(v_Position);
    vec3 R = N;
    vec3 V = R;

    const uint SAMPLE_COUNT = 1024;
    float totalWeight = 0.0;
    vec3 prefilteredColor = vec3(0.0);

    float totalMips = textureQueryLevels(u_EnvironmentMap) - 1;
    float lodLevel = u_Data.roughness * totalMips + 2.5;
    lodLevel = clamp(lodLevel, 0, totalMips);
    vec2 inputSize = vec2(textureSize(u_EnvironmentMap, 0));
    float texelSolidAngle = 4.0 * PI / (6.0 * inputSize.x * inputSize.y);

    for(uint i = 0u; i < SAMPLE_COUNT; ++i)
    {
        vec2 Xi = Hammersley(i, SAMPLE_COUNT);
        vec3 H  = ImportanceSampleGGX(Xi, N, u_Data.roughness);
        vec3 L  = normalize(2.0 * dot(V, H) * H - V);

        float NdotL = max(dot(N, L), 0.0);
        if(NdotL > 0.0)
        {
            float NdotH = max(dot(N, H), 0.0);
            float VdotH = max(dot(V, H), 0.0);
            float lodRoughness = max(u_Data.roughness, 0.04); // Use a slightly larger roughness for LOD calc

            float pdf = NdfGGX_Pdf(NdotH, lodRoughness) * 0.25;
            float sampleSolidAngle = 1.0 / (float(SAMPLE_COUNT) * pdf);
            float mipLevel = 0.5 * log2(sampleSolidAngle / texelSolidAngle) + 1;

            vec3 environmentSample = textureLod(u_EnvironmentMap, L, mipLevel).rgb * NdotL;
            environmentSample = clamp(environmentSample, 0, 65000.0f); // Clamp to the max half float value to avoid INF values

            if (any(isnan(environmentSample)) || any(isinf(environmentSample)))
            {
                continue;
            }

            prefilteredColor += environmentSample;
            totalWeight += NdotL;
        }
    }

    prefilteredColor = prefilteredColor / totalWeight;
    if (totalWeight == 0.0f) { prefilteredColor = vec3(0.0f); }

    color = vec4(prefilteredColor, 1.0f);
}