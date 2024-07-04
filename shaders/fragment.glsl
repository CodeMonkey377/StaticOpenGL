// Fragment Shader
#version 460 core
out vec4 frag_color;
flat in int tile_is_alive;

// input color depending on pos, either white or black.
void main()
{
    if (tile_is_alive > 0){
        frag_color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    }
    else{
        frag_color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    }

}