
#ifndef UTILITIES_GLSL
#define UTILITIES_GLSL

// Calculates world position from a depth value in the provided camera space
vec3 GetWorldPosition(float depth, vec2 fragPosition, mat4 inverseCameraProjection, mat4 inverseCameraView)
{
    vec3 worldPosition;

    vec4 clipSpacePosition;
    clipSpacePosition.x = (v_TextureCoord.x) * 2.0 - 1.0;
    clipSpacePosition.y = (v_TextureCoord.y) * 2.0 - 1.0;
    clipSpacePosition.z = depth;
    clipSpacePosition.w = 1.0;

    vec4 viewSpacePosition = inverseCameraProjection * clipSpacePosition;
    viewSpacePosition /= viewSpacePosition.w;

    vec4 worldPosHomogeneous = inverseCameraView * viewSpacePosition;
    worldPosition = worldPosHomogeneous.xyz;// / worldPosHomogeneous.w;

    return worldPosition;
}


mat3 CalculateTBNMatrix(vec3 normals, vec3 tangents, vec3 bitangents)
{
    vec3 N = normalize(normals);
    vec3 T = normalize(tangents);
    vec3 B = normalize(bitangents);
    T = normalize(T - N * dot(N, T));
    B = cross(N, T);
    mat3 TBN = mat3(T, B, N);
    return TBN;
}


vec3 CalculateNormalFromMap(vec3 tangentSpaceNormal, vec3 vertexNormal, vec3 vertexTangent, vec3 vertexBitangent, uint hasDirectXNormals)
{
    tangentSpaceNormal = tangentSpaceNormal * 2.0 - 1.0;

    if (hasDirectXNormals != 0)
    {
        tangentSpaceNormal.g = tangentSpaceNormal.g * -1.0f;
    }

    tangentSpaceNormal = normalize(tangentSpaceNormal);

    mat3 TBN = CalculateTBNMatrix(vertexNormal, vertexTangent, vertexBitangent);
    vec3 normal = normalize(TBN * tangentSpaceNormal);

    if (hasDirectXNormals != 0)
    {
        normal = normal * -1;
    }

    return normal;
}

#endif