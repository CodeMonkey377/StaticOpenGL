// Vertex Shader source code
#version 460 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aOffset;
layout (location = 2) in int is_alive;

flat out int tile_is_alive;

void main()
{
    vec2 translate_top_right = vec2(-95.0, -95.0);
    gl_Position = vec4(((aPos + aOffset + translate_top_right) / 100), 0.0f,  1.0f);
    tile_is_alive = is_alive;
}