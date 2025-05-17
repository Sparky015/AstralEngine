#version 450

vec2 pos[3] = vec2[3](vec2(-.5, .5), vec2(.5, .5), vec2(0.0, -.3));

void main()
{
    gl_Position = vec4(pos[gl_VertexIndex], 0.0, 1.0);
}