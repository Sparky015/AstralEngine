#version 460

layout (location = 0) out vec4 outColor;
layout (binding = 0) readonly buffer ColorBufferBlock { vec4 s_Color; } in_Color;
layout (binding = 1) readonly uniform Mult { float mult; } s_Mult;

void main()
{
    outColor = in_Color.s_Color * s_Mult.mult;
}