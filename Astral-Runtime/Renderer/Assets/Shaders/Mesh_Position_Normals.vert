#version 460

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normals;
layout(location = 2) in vec3 a_Tangents;
layout(location = 3) in vec3 a_Bitangents;

layout (set = 0, binding = 0) uniform SceneData {
    mat4 cameraViewProjection;
    mat4 cameraView;
    mat4 cameraProjection;
    mat4 inverseCameraView;
    mat4 inverseCameraProjection;
    vec2 screenSize;
    vec3 cameraPosition;
    uint numLights;
} u_SceneData;

layout (push_constant) uniform ModelTransform { mat4 transform; } u_ModelProjection;

layout(location = 0) out vec3 v_WorldPosition;
layout(location = 1) out vec3 v_Normals;
layout(location = 2) out vec3 v_Tangents;
layout(location = 3) out vec3 v_Bitangents;

void main()
{

    mat3 normalMatrix = mat3(transpose(inverse(u_ModelProjection.transform)));
    v_Normals = normalize(normalMatrix * a_Normals);
    v_Tangents = normalize(normalMatrix * a_Tangents);
    v_Bitangents = normalize(normalMatrix * a_Bitangents);

    vec4 worldPositionHomogeneous = u_ModelProjection.transform * vec4(a_Position, 1.0f);
    v_WorldPosition = worldPositionHomogeneous.xyz;
    gl_Position = u_SceneData.cameraViewProjection * vec4(v_WorldPosition, 1.0f);
}
