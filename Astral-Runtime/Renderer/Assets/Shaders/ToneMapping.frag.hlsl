
Texture2D<float4> u_SceneRender : register(t0, space1);
SamplerState u_SceneRenderSampler : register(s0, space1);
Texture3D<float4> u_ToneMapLut : register(t0, space2); // LUT is ACES baked lut converting ACEScg to sRGB
SamplerState u_ToneMapLutSampler : register(s0, space2);
Texture1D<float4> u_ToneMapShaper : register(t1, space2); // Shaper is ACES baked shaper for helping convert ACEScg to sRGB
SamplerState u_ToneMapShaperSampler : register(s1, space2);


uint ToneMappingDebugView_DEBUG_VIEW_REINHARD = 1;
uint ToneMappingDebugView_DEBUG_VIEW_NO_TONE_MAPPING = 2;

struct PushConstantData
{
    float exposure;
    float shaperInputMin;
    float shaperInputMax;
    uint toneMappingDebugView;
};

[[vk::push_constant]]
PushConstantData u_PushConstantData;

struct PS_INPUT 
{
    float2 textureCoord : LOCATION4;
};

#include "ColorTransforms.hlsl"

float4 main(PS_INPUT input) : SV_Target0
{
    float3 renderedColors = u_SceneRender.Sample(u_SceneRenderSampler, input.textureCoord).rgb;
    renderedColors *= exp2(u_PushConstantData.exposure);

    if (u_PushConstantData.toneMappingDebugView == ToneMappingDebugView_DEBUG_VIEW_NO_TONE_MAPPING)
    {
        return float4(ConvertAP1PrimariesToSRGBPrimaries(renderedColors), 1.0);
    }
    else if (u_PushConstantData.toneMappingDebugView == ToneMappingDebugView_DEBUG_VIEW_REINHARD)
    {
        renderedColors = renderedColors / (1 + renderedColors);
        return float4(ConvertAP1PrimariesToSRGBPrimaries(renderedColors), 1.0);
    }

    renderedColors = clamp(renderedColors, u_PushConstantData.shaperInputMin, u_PushConstantData.shaperInputMax); // Clamp to the max float value to avoid INF values going through the fit
    float shaperRange = u_PushConstantData.shaperInputMax - u_PushConstantData.shaperInputMin;
    float3 shaperCoords = (renderedColors - u_PushConstantData.shaperInputMin) / shaperRange;

    shaperCoords = clamp(shaperCoords, float3(0.0, 0.0, 0.0), float3(1.0, 1.0, 1.0));

    float3 shapedCoords = float3(
        u_ToneMapShaper.Sample(u_ToneMapShaperSampler, shaperCoords.r).r,
        u_ToneMapShaper.Sample(u_ToneMapShaperSampler, shaperCoords.g).r,
        u_ToneMapShaper.Sample(u_ToneMapShaperSampler, shaperCoords.b).r
    );

    shapedCoords = clamp(shapedCoords, 0, 1);

    float3 toneMappedData = u_ToneMapLut.SampleLevel(u_ToneMapLutSampler, shapedCoords, 0).rgb;

    return float4(toneMappedData, 1.0);
}