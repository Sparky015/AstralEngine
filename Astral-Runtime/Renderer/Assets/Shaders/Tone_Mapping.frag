#version 460

layout(location = 4) in vec2 v_TextureCoord;

layout (set = 1, binding = 0) uniform sampler2D u_SceneRender;
layout (set = 2, binding = 0) uniform sampler3D u_ToneMapLut; // LUT is ACES baked lut converting ACEScg to rec709 linear

layout (push_constant) uniform PushConstantData {
    float exposure;
} u_PushConstantData;

layout(location = 0) out vec4 color;


// ACES (Narkowicz)
vec3 ACESFit(vec3 pixelColor)
{
    vec3 a = pixelColor * (pixelColor + 0.0245786) - 0.000090537;
    vec3 b = pixelColor * (0.983729 * pixelColor + 0.4329510) + 0.238081;
    return a / b;
}

vec3 toneMapACES(vec3 pixelColor)
{
    pixelColor = clamp(pixelColor, 0, 0x7F7FFFFF); // Clamp to the max float value to avoid INF values going through the fit
    pixelColor = ACESFit(pixelColor);
    return clamp(pixelColor, 0.0, 1.0);
}

void main()
{
    vec3 scenePixelData = texture(u_SceneRender, v_TextureCoord).rgb;
    scenePixelData *= exp2(u_PushConstantData.exposure);

    vec3 toneMappedData = toneMapACES(scenePixelData);

    toneMappedData = pow(toneMappedData, vec3(1.0f / 2.2f));

    color = vec4(toneMappedData, 1.0);
}


//void main()
//{
//    vec3 renderedColors = texture(u_SceneRender, v_TextureCoord).rgb;
//    color = texture(u_ToneMapLut, renderedColors);
//}
