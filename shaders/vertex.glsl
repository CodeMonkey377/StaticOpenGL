// Vertex Shader source code
#version 460 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aOffset;
layout (location = 2) in int is_alive;

uniform int map_width;
uniform int map_height;

flat out int tile_is_alive;

void main()
{
    // map offset
    vec2 translate_top_right = vec2(-map_width + 1, -map_height + 1);
    // scales map to center
    vec2 scale = vec2(map_width, map_height);
    gl_Position = vec4(((aPos + aOffset + translate_top_right) / scale), 0.0f,  1.0f);
    tile_is_alive = is_alive;
}