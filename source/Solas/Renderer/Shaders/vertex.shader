#shader vertex

#version 410

layout(location = 0) in vec4 position;

void main()
{
    v_Position = position;
    gl_Position = position;
}


