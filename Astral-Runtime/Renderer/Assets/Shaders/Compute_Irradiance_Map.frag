#version 460

layout(location = 0) in vec3 v_Position;
layout(location = 1) in vec3 v_Normals;
layout(location = 2) in vec3 v_Tangents;
layout(location = 3) in vec3 v_Bitangents;

layout (set = 1, binding = 0) uniform samplerCube u_Cubemap;
layout (set = 1, binding = 1) uniform samplerCube u_Irradiance;

layout(location = 0) out vec4 color;


const float PI = 3.14159265359;


void main()
{
    vec3 normal = normalize(v_Position);
    vec3 irradiance = vec3(0.0);

    vec3 up    = vec3(0.0, 1.0, 0.0);
    vec3 right = normalize(cross(up, normal));
    up         = normalize(cross(normal, right));

    float sampleDelta = 0.01;
    float nrSamples = 0.0;
    for(float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta)
    {
        for(float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta)
        {
            // spherical to cartesian (in tangent space)
            vec3 tangentSample = vec3(sin(theta) * cos(phi),  sin(theta) * sin(phi), cos(theta));

            // tangent space to world
            vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * normal;

            vec3 cubemapSample = texture(u_Cubemap, sampleVec).rgb * cos(theta) * sin(theta);
            cubemapSample = clamp(cubemapSample, vec3(0), vec3(0x7F7FFFFF)); // Clamp to the max float value to avoid INF values
            if (cubemapSample.x == 0x7F7FFFFF) { cubemapSample.x = 0; } // Remove the INFs influence on the irradiance calcs
            if (cubemapSample.y == 0x7F7FFFFF) { cubemapSample.y = 0; }
            if (cubemapSample.z == 0x7F7FFFFF) { cubemapSample.z = 0; }


            irradiance += cubemapSample;
            nrSamples++;
        }
    }
    irradiance = PI * irradiance * (1.0 / float(nrSamples));

    irradiance = clamp(irradiance, 0, 0x7F7FFFFF); // Clamp to the max float value to avoid INF values
    color = vec4(irradiance, 1.0f);
}