#version 460

layout(location = 4) in vec2 v_TextureCoord;

layout (set = 1, binding = 0) uniform sampler2D u_SceneRender;
layout (set = 2, binding = 0) uniform sampler3D u_ToneMapLut; // LUT is ACES baked lut converting ACEScg to sRGB
layout (set = 2, binding = 1) uniform sampler1D u_ToneMapShaper; // Shaper is ACES baked shaper for helping convert ACEScg to sRGB


layout (push_constant) uniform PushConstantData {
    float exposure;
    float shaperInputMin;
    float shaperInputMax;
} u_PushConstantData;

layout(location = 0) out vec4 color;


//vec3 ConvertAP1PrimariesToSRGBPimaries(vec3 srgbPrimaries)
//{
//    const mat3 srgbPrimariesToAP1Primaries = mat3( 0.613132422390542, 0.070124380833917, 0.020587657528185,
//    0.339538015799666, 0.916394011313573, 0.109574571610682,
//    0.047416696048269, 0.013451523958235, 0.869785404035327 );
//    return inverse(srgbPrimariesToAP1Primaries) * srgbPrimaries;
//}
//

//// ACES (Narkowicz)
//vec3 ACESFit(vec3 pixelColor)
//{
//    vec3 a = pixelColor * (pixelColor + 0.0245786) - 0.000090537;
//    vec3 b = pixelColor * (0.983729 * pixelColor + 0.4329510) + 0.238081;
//    return a / b;
//}
//
//vec3 toneMapACES(vec3 pixelColor)
//{
//    pixelColor = clamp(pixelColor, 0, 0x7F7FFFFF); // Clamp to the max float value to avoid INF values going through the fit
//    pixelColor = ACESFit(pixelColor);
//    return clamp(pixelColor, 0.0, 1.0);
//}
//
//void main()
//{
//    vec3 scenePixelData = texture(u_SceneRender, v_TextureCoord).rgb;
//    scenePixelData *= exp2(u_PushConstantData.exposure);
//
//    vec3 toneMappedData = toneMapACES(scenePixelData);
//    toneMappedData = ConvertAP1PrimariesToSRGBPimaries(toneMappedData);
//
//    toneMappedData = pow(toneMappedData, vec3(1.0f / 2.2f));
//
//    color = vec4(toneMappedData, 1.0);
//}


void main()
{
    vec3 renderedColors = texture(u_SceneRender, v_TextureCoord).rgb;
    renderedColors *= exp2(u_PushConstantData.exposure);

    renderedColors = clamp(renderedColors, u_PushConstantData.shaperInputMin, u_PushConstantData.shaperInputMax); // Clamp to the max float value to avoid INF values going through the fit
    float shaperRange = u_PushConstantData.shaperInputMax - u_PushConstantData.shaperInputMin;
    vec3 shaperCoords = (renderedColors - u_PushConstantData.shaperInputMin) / shaperRange;

    shaperCoords = clamp(shaperCoords, vec3(0), vec3(1));

    vec3 shapedCoords = vec3(
        texture(u_ToneMapShaper, shaperCoords.r).r,
        texture(u_ToneMapShaper, shaperCoords.g).r,
        texture(u_ToneMapShaper, shaperCoords.b).r
    );

    shapedCoords = clamp(shapedCoords, 0, 1);

    vec3 toneMappedData = textureLod(u_ToneMapLut, shapedCoords, 0).rgb;

    color = vec4(toneMappedData, 1.0);
}
