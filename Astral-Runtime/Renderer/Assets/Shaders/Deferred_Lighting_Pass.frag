
layout (set = 0, binding = 0) uniform SceneData {
    mat4 cameraViewProjection;
    mat4 inverseCameraView;
    mat4 inverseCameraProjection;
    vec2 screenSize;
    vec3 cameraPosition;
    uint numLights;
} u_SceneData;

struct Light {
    vec3 lightPosition;
    vec3 lightColor;
};

layout (set = 0, binding = 1) readonly buffer Lights {
    Light[] lights;
} u_SceneLights;

layout(input_attachment_index = 0, set = 1, binding = 0) uniform subpassInput u_AlbedoInput;
layout(input_attachment_index = 1, set = 1, binding = 1) uniform subpassInput u_MetallicInput;
layout(input_attachment_index = 2, set = 1, binding = 2) uniform subpassInput u_RoughnessInput;
layout(input_attachment_index = 3, set = 1, binding = 3) uniform subpassInput u_EmissionInput;
layout(input_attachment_index = 4, set = 1, binding = 4) uniform subpassInput u_NormalInput;
layout(input_attachment_index = 5, set = 1, binding = 5) uniform subpassInput u_DepthBufferInput;

layout(location = 0) out vec4 outColor;

// Find world position of frag from depth buffer
vec3 GetWorldPosition()
{
    vec3 worldPosition;

    vec2 pixelCoords = gl_FragCoord.xy;
    float depth = subpassLoad(u_DepthBufferInput).r;
    vec4 clipSpacePosition;
    clipSpacePosition.x = (gl_FragCoord.x / u_SceneData.screenSize.x) * 2.0 - 1.0;
    clipSpacePosition.y = (gl_FragCoord.y / u_SceneData.screenSize.y) * 2.0 - 1.0;
    clipSpacePosition.z = depth * 2.0 - 1.0;
    clipSpacePosition.w = 1.0;
    vec4 viewSpacePosition = u_SceneData.inverseCameraProjection * clipSpacePosition;
    viewSpacePosition /= viewSpacePosition.w;
    vec4 worldPosHomogeneous = u_SceneData.inverseCameraView * viewSpacePosition;
    worldPosition = worldPosHomogeneous.xyz / worldPosHomogeneous.w;

    return worldPosition;
}

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
    vec3 baseColor = subpassLoad(u_AlbedoInput).rgb;
    float metallic = subpassLoad(u_MetallicInput).r;
    float roughness = subpassLoad(u_RoughnessInput).r;
    vec3 emission = subpassLoad(u_EmissionInput).rgb;
    vec3 normal = subpassLoad(u_NormalInput).rgb;
    vec3 worldPosition = GetWorldPosition();

    vec3 cameraPosition = u_SceneData.cameraPosition;
    vec3 finalLight = vec3(0.0f);

    if (u_SceneData.numLights == 0)
    {
        outColor = vec4(emission, 1.0f);
    }

    for (int i = 0; i < u_SceneData.numLights; i++)
    {
        vec3 lightPosition = u_SceneLights.lights[i].lightPosition;
        vec3 lightColor = u_SceneLights.lights[i].lightColor;

        // Vectors
        vec3 N = normalize(normal);
        vec3 V = normalize(cameraPosition - worldPosition);
        vec3 L = normalize(lightPosition - worldPosition);
        vec3 H = normalize(V + L);


        // PBR Equation in Full
        vec3 F0 = vec3(0.04);
        float alpha = pow(roughness, 2.0f);
        F0 = mix(F0, baseColor, metallic);
        vec3 Ks = F(F0, V, H);
        vec3 Kd = (vec3(1.0) - Ks) * (1.0f - metallic);

        vec3 lambert = baseColor / 3.1415;

        vec3 cookTorranceNumerator = D(alpha, N, H) * G(alpha, N, V, L) * F(F0, V, H);
        float cookTorranceDenominator = 4.0 * max(dot(V, N), 0.0) * max(dot(L, N), 0.0);
        cookTorranceDenominator = max(cookTorranceDenominator, 0.000001);
        vec3 cookTorrance = cookTorranceNumerator / cookTorranceDenominator;

        vec3 BRDF = Kd * lambert + cookTorrance;
        vec3 outgoingLight = emission + BRDF * lightColor * max(dot(L, N), 0.0);
        finalLight += outgoingLight;
    }

    outColor = vec4(finalLight, 1.0f);
}
