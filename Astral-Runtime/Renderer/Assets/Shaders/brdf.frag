#version 460

layout(location = 0) in vec3 v_WorldPosition;
layout (location = 1) in vec2 v_TextureCoord;
layout (location = 2) in vec3 v_Normals;

layout (set = 0, binding = 0) uniform SceneData {
    mat4 cameraViewProjection;
    vec3 cameraPosition;
    vec3 lightPosition;
    vec3 lightColor;
} u_SceneData;

layout (set = 1, binding = 0) uniform sampler2D u_BaseColor;
layout (set = 1, binding = 1) uniform sampler2D u_Metallic;
layout (set = 1, binding = 2) uniform sampler2D u_Roughness;
layout (set = 1, binding = 3) uniform sampler2D u_Emission;
layout (set = 1, binding = 4) uniform sampler2D u_Normals;

layout (push_constant) uniform ModelTransform { mat4 transform; } u_ModelProjection;

layout(location = 0) out vec4 color;


// GGX/Trowbridge-Reitz Normal Distribution Function
float D(float alpha, vec3 N, vec3 H)
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
float G(float alpha, vec3 N, vec3 V, vec3 L)
{
    return G1(alpha, N, V) * G1(alpha, N, L);
}

// Fresnel-Schlick Function
vec3 F(vec3 F0, vec3 V, vec3 H)
{
    return F0 + (vec3(1.0) - F0) * pow(1 - max(dot(V, H), 0.0), 5.0);
}


void main()
{
    vec3 baseColor = texture(u_BaseColor, v_TextureCoord).rgb;
    vec3 metallic = texture(u_Metallic, v_TextureCoord).rgb;
    float roughness = texture(u_Roughness, v_TextureCoord).r;
    vec3 emission = texture(u_Emission, v_TextureCoord).rgb;
    vec3 normal = texture(u_Normals, v_TextureCoord).rgb;
    normal = mat3(u_ModelProjection.transform) * normal;

    vec3 cameraPosition = u_SceneData.cameraPosition;
    vec3 lightPosition = u_SceneData.lightPosition;
    lightPosition.x = lightPosition.x * -1;
    vec3 lightColor = u_SceneData.lightColor;

    // Vectors
    vec3 N = normalize(v_Normals);
    vec3 V = normalize(cameraPosition - v_WorldPosition);
    vec3 L = normalize(lightPosition - v_WorldPosition);
    vec3 H = normalize(V + L);


    // PBR Equation in Full
    vec3 F0 = vec3(0.04);
    float alpha = pow(roughness, 2.0f);
    F0 = mix(F0, baseColor, metallic.r); // Mix based on metallic value
    vec3 Ks = F(F0, V, H);
    vec3 Kd = (vec3(1.0) - Ks) * (1.0f - metallic.r);

    vec3 lambert = baseColor / 3.1415;

    vec3 cookTorranceNumerator = D(alpha, N, H) * G(alpha, N, V, L) * F(F0, V, H);
    float cookTorranceDenominator = 4.0 * max(dot(V, N), 0.0) * max(dot(L, N), 0.0);
    cookTorranceDenominator = max(cookTorranceDenominator, 0.000001);
    vec3 cookTorrance = cookTorranceNumerator / cookTorranceDenominator;

    vec3 BRDF = Kd * lambert + cookTorrance;
    vec3 outgoingLight = emission + BRDF * lightColor * max(dot(L, N), 0.0);

    color = vec4(outgoingLight, 1.0f);
}
