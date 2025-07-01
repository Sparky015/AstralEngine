#version 460

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normals;
layout(location = 2) in vec3 a_Tangents;
layout(location = 3) in vec3 a_Bitangents;
layout(location = 4) in vec2 a_TextureCoord;

layout (set = 0, binding = 0) uniform SceneData {
    mat4 cameraViewProjection;
    vec3 cameraPosition;
    uint numLights;
} u_SceneData;

layout (push_constant) uniform ModelTransform { mat4 transform; } u_ModelProjection;

layout (location = 0) out vec2 v_TextureCoord;

void main()
{
    v_TextureCoord = a_TextureCoord;
    gl_Position = u_SceneData.cameraViewProjection * u_ModelProjection.transform * vec4(a_Position, 1.0f);
}
