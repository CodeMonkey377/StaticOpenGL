// Vertex Shader source code
#version 460 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aOffset;

void main()
{
    vec2 translate_top_right = vec2(0.0, 0.0);
    gl_Position = vec4(((aPos + aOffset + translate_top_right) / 100), 0.0f,  1.0f);
}