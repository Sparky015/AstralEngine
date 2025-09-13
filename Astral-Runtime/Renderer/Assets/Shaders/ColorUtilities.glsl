
#ifndef COLOR_UTILITIES_GLSL
#define COLOR_UTILITIES_GLSL

vec3 ConvertSRGBPimariesToAP1Primaries(vec3 srgbPrimaries)
{
    const mat3 srgbPrimariesToAP1Primaries = mat3( 0.613132422390542, 0.070124380833917, 0.020587657528185,
    0.339538015799666, 0.916394011313573, 0.109574571610682,
    0.047416696048269, 0.013451523958235, 0.869785404035327 );
    return srgbPrimariesToAP1Primaries * srgbPrimaries;
}

#endif