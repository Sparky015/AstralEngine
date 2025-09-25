
#ifndef COLOR_TRANSFORMS_GLSL
#define COLOR_TRANSFORMS_GLSL

// Converts linear sRGB primaries to linear AP1 pimaries
vec3 ConvertSRGBPimariesToAP1Primaries(vec3 srgbPrimaries)
{
    const mat3 srgbPrimariesToAP1Primaries = mat3( 0.613132422390542, 0.070124380833917, 0.020587657528185,
                                                   0.339538015799666, 0.916394011313573, 0.109574571610682,
                                                   0.047416696048269, 0.013451523958235, 0.869785404035327 );
    return srgbPrimariesToAP1Primaries * srgbPrimaries;
}

// Converts linear sRGB primaries to linear AP1 pimaries
vec3 ConvertAP1PrimariesToSRGBPrimaries(vec3 ap1Primaries)
{

    const mat3 AP1PrimariesToSRGBPrimaries = mat3( 1.70487332344055176, -0.13010872900485992, -0.02396308258175850,
                                                  -0.62171816825866699, 1.14070236682891846, -0.12898841500282288,
                                                  -0.08332676440477371, -0.01054841559380293, 1.15301001071929932 );
    return AP1PrimariesToSRGBPrimaries * ap1Primaries;
}

#endif