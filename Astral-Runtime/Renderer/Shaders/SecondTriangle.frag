#version 460

layout (location = 0) out vec4 outColor;
layout (location = 1) in vec2 texCoord;
layout (binding = 0) readonly buffer ColorBufferBlock { vec4 s_Color; } in_Color;
layout (binding = 1) readonly uniform Mult { float mult; } s_Mult;
layout (binding = 2) uniform sampler2D u_Sampler;
//in_Color.s_Color * s_Mult.mult



void main()
{
    outColor = texture(u_Sampler, texCoord);
}