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

layout(location = 0) out vec3 v_Position;
layout(location = 1) out vec3 v_Normals;
layout(location = 2) out vec3 v_Tangents;
layout(location = 3) out vec3 v_Bitangents;

void main()
{
    v_Position = a_Position;
    v_Normals = a_Normals;
    v_Tangents = a_Tangents;
    v_Bitangents = a_Bitangents;

    mat4 view = mat4(mat3(u_SceneData.cameraView));

    vec4 position = u_SceneData.cameraProjection * view * vec4(a_Position, 1.0f);
    gl_Position = position.xyww;
}
