// Fragment Shader
#version 460 core
out vec4 frag_color;
flat in int tile_is_alive;
flat in int pause_state;

// input color depending on pos, either white or black.
void main()
{

    if (tile_is_alive > 0){
        if (pause_state > 0){
            frag_color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
        }
        else {
            frag_color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
        }
    }
    else{
        frag_color = vec4(0.2f, 0.3f, 0.2f, 1.0f);
    }

    /*
    // Encode tile_is_alive into the red channel
    float r = float(tile_is_alive) / 255.0;
    frag_color = vec4(r, 0.0, 0.0, 1.0);
    */
}