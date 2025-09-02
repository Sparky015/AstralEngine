#version 460

layout(location = 4) in vec2 v_TextureCoord;

layout(set = 1, binding = 0) uniform sampler2D u_SceneInput;

layout(location = 0) out vec4 color;


float GetLuminance(vec4 rgba)
{
    return dot(rgba.rgb, vec3(0.299, 0.587, 0.114));
}

#define FXAA_SEARCH_STEPS 32
#define FXAA_SEARCH_THRESHOLD 0.125
#define FXAA_SUBPIX_QUALITY 0.75
#define FXAA_EDGE_THRESHOLD 0.0625
#define FXAA_EDGE_THRESHOLD_MIN 0.0312

void main()
{
    // Reference: NVIDIA FXAA 3.11

    ivec2 textureSize = textureSize(u_SceneInput, 0);
    vec2 texelSize = vec2(1.0 / textureSize.x, 1.0 / textureSize.y);

    vec2 middleUV = v_TextureCoord;

    /*--------------------------------------------------------------------------*/

    // Constrast & Edge Detection

    vec3 rgbM = texture(u_SceneInput, middleUV).rgb;
    float lumaM = GetLuminance(vec4(rgbM, 1.0));

    float lumaNW = GetLuminance(texture(u_SceneInput, middleUV + texelSize * vec2(-1.0, 1.0)));
    float lumaNE = GetLuminance(texture(u_SceneInput, middleUV + texelSize * vec2(1.0, 1.0)));
    float lumaSW = GetLuminance(texture(u_SceneInput, middleUV + texelSize * vec2(-1.0, -1.0)));
    float lumaSE = GetLuminance(texture(u_SceneInput, middleUV + texelSize * vec2(1.0, -1.0)));

    float lumaMax = max(lumaM, max(max(lumaNW, lumaNE), max(lumaSW, lumaSE)));
    float lumaMin = min(lumaM, min(min(lumaNW, lumaNE), min(lumaSW, lumaSE)));
    float lumaRange = lumaMax - lumaMin;

    if (lumaRange < max(qualityEdgeThresholdMin, lumaMax * qualityEdgeThreshold))
    {
        color = vec4(rgbM, 1.0);
        return;
    }

    /*--------------------------------------------------------------------------*/

    // Edge Direction

    float lumaN =  GetLuminance(texture(u_SceneInput, middleUV + texelSize * vec2(0.0, 1.0)));
    float lumaE =  GetLuminance(texture(u_SceneInput, middleUV + texelSize * vec2(1.0, 0.0)));
    float lumaS =  GetLuminance(texture(u_SceneInput, middleUV + texelSize * vec2(0.0, -1.0)));
    float lumaW =  GetLuminance(texture(u_SceneInput, middleUV + texelSize * vec2(-1.0, 0.0)));

    float edgeHorizontal = abs((lumaNW + lumaNE) - (lumaSW + lumaSE)) + abs(lumaN - lumaS) * 2.0;
    float edgeVertical = abs((lumaNW + lumaSW) - (lumaNE + lumaSE)) + abs(lumaW - lumaE) * 2.0;
    bool isHorizontal = edgeHorizontal >= edgeVertical;


    /*--------------------------------------------------------------------------*/

    // Edge Endpoint Search

    vec2 offNP = isHorizontal ? vec2(0.0, texelSize.y) : vec2(texelSize.x, 0.0);
    vec2 offDir = isHorizontal ? vec2(texelSize.x, 0.0) : vec2(0.0, texelSize.y);

    // Starting search points, moved 0.5 pixels into the edge
    vec2 posP = middleUV + offDir * 0.5;
    vec2 posN = middleUV - offDir * 0.5;

    // Luminance at the gradient ends
    float lumaEndP = GetLuminance(texture(u_SceneInput, posP));
    float lumaEndN = GetLuminance(texture(u_SceneInput, posN));

    for (int i = 0; i < FXAA_SEARCH_STEPS; i++)
    {
        float luma = GetLuminance(texture(u_SceneInput, posP));
        if (abs(luma - lumaM) > lumaRange * FXAA_SEARCH_THRESHOLD) break;
        posP += offDir;
    }
    for (int i = 0; i < FXAA_SEARCH_STEPS; i++)
    {
        float luma = GetLuminance(texture(u_SceneInput, posN));
        if (abs(luma - lumaM) > lumaRange * FXAA_SEARCH_THRESHOLD) break;
        posN -= offDir;
    }

    /*--------------------------------------------------------------------------*/

    // Blending

    // Calculate distance to endpoints
    float distP = isHorizontal ? (posP.x - middleUV.x) : (posP.y - middleUV.y);
    float distN = isHorizontal ? (middleUV.x - posN.x) : (middleUV.y - posN.y);

    // Choose the shorter distance and calculate the blend offset
    float shorterDist = min(distP, distN);
    float longerDist = max(distP, distN);
    float directionSign = (lumaEndP < lumaEndN) ? 1.0 : -1.0;

    // If the edge is very thin, reduce the sub-pixel blending to avoid artifacts
    float subpixBlend = max(0.0, (shorterDist / longerDist) - FXAA_SUBPIX_QUALITY) * directionSign;

    vec2 finalPos = middleUV + offNP * subpixBlend;
    color = vec4(texture(u_SceneInput, finalPos).rgb, 1.0);
}