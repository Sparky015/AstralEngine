#version 460

struct VertexData
{
    float x, y, z;
    float u, v;
};

layout (binding = 0) readonly buffer Vertices { VertexData data[]; } inVertices;

void main()
{
    VertexData vertex = inVertices.data[gl_VertexIndex];
    vec3 position = vec3(vertex.x, vertex.y, vertex.z);
    gl_Position = vec4(position, 1.0);
}