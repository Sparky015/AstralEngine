
TextureCube<float4> u_EnvironmentMap : register(t0, space0);
SamplerState u_Sampler : register(s0);
RWTexture2DArray<half4> u_IrradianceMap : register(u1, space0);

struct PushConstantData
{
    float faceIndex;
};

[[vk::push_constant]]
PushConstantData pushConstants;

[numthreads(8, 8, 1)]
void main(
    uint3 dispatchThreadID : SV_DispatchThreadID
)
{
    const float PI = 3.14159265359;

    int3 outputSize;
    u_IrradianceMap.GetDimensions(outputSize.x, outputSize.y, outputSize.z);

    uint2 pixelCoords = dispatchThreadID.xy;
    if (pixelCoords.x >= outputSize.x || pixelCoords.y >= outputSize.y) { return; }

    float2 uv = (float2(pixelCoords) + 0.5) / outputSize.x;
    uv = uv * 2.0 - 1.0; // Move from [0, 1] to [-1, 1]

    float3 normal;

    int faceIndex = int(pushConstants.faceIndex);

    switch (faceIndex)
    {
        case 0: normal = float3( 1.0, -uv.y, -uv.x); break; // +X
        case 1: normal = float3(-1.0, -uv.y,  uv.x); break; // -X
        case 2: normal = float3( uv.x,  1.0,  uv.y); break; // +Y
        case 3: normal = float3( uv.x, -1.0, -uv.y); break; // -Y
        case 4: normal = float3( uv.x, -uv.y,  1.0); break; // +Z
        case 5: normal = float3(-uv.x, -uv.y, -1.0); break; // -Z
    }

    normal = normalize(normal);

    float3 irradiance = float3(0.0, 0.0, 0.0);

    float3 up    = float3(0.0, 1.0, 0.0);
    float3 right = normalize(cross(up, normal));
    up         = normalize(cross(normal, right));

    float sampleDelta = 0.02;
    float nrSamples = 0.0;
    for(float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta)
    {
        for(float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta)
        {
            // spherical to cartesian (in tangent space)
            float3 tangentSample = float3(sin(theta) * cos(phi),  sin(theta) * sin(phi), cos(theta));

            // tangent space to world
            float3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * normal;

            float3 cubemapSample = u_EnvironmentMap.SampleLevel(u_Sampler, sampleVec, 4.0f).rgb * cos(theta) * sin(theta);

            if (any(isnan(cubemapSample)) || any(isinf(cubemapSample)))
            {
                continue; // Skip this sample
            }

            irradiance += cubemapSample;
            nrSamples++;
        }
    }
    irradiance = PI * irradiance * (1.0 / float(nrSamples));

    irradiance = clamp(irradiance, 0, 0x7F7FFFFF); // Clamp to the max float value to avoid INF values

    u_IrradianceMap[int3(pixelCoords, pushConstants.faceIndex)] = float4(irradiance, 1.0);
}