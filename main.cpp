#include "glad/include/glad/glad.h"
#include "glfw-3.4/include/GLFW/glfw3.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

/// Shader file reader function declaration
std::string readFile(const char* filePath){
    std::ifstream file(filePath);
    if(!file.is_open()) {
        std::cerr << "Failed to open shader file: " << filePath << "\n";
        exit(EXIT_FAILURE);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}
void window_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}
/// function used in render loop to update is_alive vertex attribute sent to fragment shader
void update_game(std::vector<GLuint> &tile_is_alive, int map_width, int map_height){
    int live_neighbors;
    int i;

    int top_left_index;
    int top_index;
    int top_right_index;
    int left_index;
    int right_index;
    int bottom_left_index;
    int bottom_index;
    int bottom_right_index;

    std::vector<int> neighbor_count;

    // loop over every point, check order - TL, L, BL, T, B, TR, R, BR
    for (i = 0; i < map_width * map_height - 1; i++) {
        live_neighbors = 0;
        /// Indices
        top_left_index = (i + map_width - 1);
        top_index = (i + map_width);
        top_right_index = (i + map_width + 1);
        left_index = (i - 1);
        right_index = (i + 1);
        bottom_left_index = (i - map_width - 1);
        bottom_index = (i - map_width);
        bottom_right_index = (i - map_width + 1);

        // left wall - enter loop if not left
        if (i != 0 && i % map_width != 0) {
            // top wall - enter loop if not top
            if (i < map_width * map_height - map_width) {
                /// top left check
                if (tile_is_alive[top_left_index] == 1) {
                    live_neighbors++;
                }
            }
            /// left check
            if (tile_is_alive[left_index] == 1) {
                live_neighbors++;
            }
            // bottom wall - enter loop if not bottom
            if (i > map_width - 1) {
                /// bottom left check
                if (tile_is_alive[bottom_left_index] == 1) {
                    live_neighbors++;
                }
            }
        }
        // top wall - enter loop if not top
        if (i < map_width * map_height - map_width) {
            /// top check
            if (tile_is_alive[top_index] == 1) {
                live_neighbors++;
            }
        }
        // bottom wall - enter loop if not bottom
        if (i > map_width - 1) {
            /// bottom check
            if (tile_is_alive[bottom_index] == 1) {
                live_neighbors++;
            }
        }
        // right wall - enter loop if not right
        if (i != map_width - 1 && i + 1 % map_width != 0) {
            // top wall - enter loop if not top
            if (i < map_width * map_height - map_width) {
                /// top right check
                if (tile_is_alive[top_right_index] == 1) {
                    live_neighbors++;
                }
            }
            /// right check
            if (tile_is_alive[right_index] == 1) {
                live_neighbors++;
            }
            // bottom wall - enter loop if not bottom
            if (i > map_width - 1) {
                /// bottom right check
                if (tile_is_alive[bottom_right_index] == 1) {
                    live_neighbors++;
                }
            }
        }
/*
        if (i == 25){
            std::cout << "  -  Live neighbors for 25 = " << live_neighbors << "\n";
        }
        std::cout << "i = " << i << " - Live neighbors = " << live_neighbors <<"\n";
*/
        neighbor_count.push_back(live_neighbors);
    }

    for (int j = 0; j < map_width * map_height - 1; j++){
        if (neighbor_count[j] < 2){
            tile_is_alive[j] = 0;
        }
        else if (neighbor_count[j] == 3){
            tile_is_alive[j] = 1;
        }
        else if (neighbor_count[j] > 3){
            tile_is_alive[j] = 0;
        }
    }

}

void click_input(std::vector<GLuint> &tile_is_alive,
                 const int &xpos,const int &ypos,
                 const int &map_width, const int &map_height,
                 const int &screen_width, const int &screen_height){
    /// M2S or map size to screen size ratio's
    float screen_tile_width = float(screen_width) / float(map_width);
    float screen_tile_height = float(screen_height) / float(map_height);

    //std::cout << "tile width - " << screen_tile_width << ", tile height - "

    for (int i = 0; i < tile_is_alive.size(); i++){
        /// row number 0 - map width
        if (i / map_width == int(float(screen_height - ypos) / screen_tile_height)){
            //std::cout << "X is TRUE :: ";
            /// col number 0 - map height
            if (i % map_width == int(xpos / screen_tile_width)){
                //std::cout << "Y is TRUE :: ";
                tile_is_alive[i] = 1;
            }
        }
        //std::cout << "i = " << i << "  -  i / mapw: " << i / map_width << "  - ypos / tile height: " << int(ypos / screen_tile_height) << "\n";
    }
    //std::cout << "Click function just ran!\n";
}

void clear_map(std::vector<GLuint> &tile_is_alive){
    tile_is_alive.assign(tile_is_alive.size(), 0);
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    int map_width = 25;
    int map_height = 25;
    int window_width = 2000;
    int window_height = 2000;


    // Two Triangles that make up square
    GLfloat vertices[12] = {
            1.0f, 1.0f,
            -1.0f, 1.0f,
            -1.0f, -1.0f,

            -1.0f, -1.0f,
            1.0f, -1.0f,
            1.0f, 1.0,
    };
    // vector filled with the offset values of each instance
    std::vector<glm::vec2> tile_offset;
    int index_x = 0;
    int index_y = 0;
    for (int i = 0; i < map_height; i++){
        for (int j = 0; j < map_width; j++){
            tile_offset.emplace_back(index_x, index_y);
            index_x += 2;
        }
        index_x = 0;
        index_y += 2;
    }
    // vector that conveys weather a given tile is alive or dead. Runs bottom left to top right
    std::vector<GLuint> tile_is_alive(map_width * map_height, 0);

    GLFWwindow* window = glfwCreateWindow(window_width, window_height, "Static OpenGL", nullptr, nullptr);
    if (window == nullptr){
        std::cout << "Failed to create GLFW window" << "\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

   if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)){
       std::cout << "Glad failed to load GL function pointers" << "\n";
       return -2;
   }
   std::cout << "\nLoaded Open GL\n";
   glViewport(0, 0, window_width, window_height);

    std::string vertex_shader_code = readFile("../shaders/vertex.glsl");
    const GLchar *vertex_shader_source = vertex_shader_code.c_str();
    std::string fragment_shader_code = readFile("../shaders/fragment.glsl");
    const GLchar *fragment_shader_source = fragment_shader_code.c_str();

    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, nullptr);
    glCompileShader(vertex_shader);
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, nullptr);
    glCompileShader(fragment_shader);

    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    // Sets uniform values in vertex shader for map offset
    glUseProgram(shader_program);
    glUniform1i(glGetUniformLocation(shader_program, "map_width"), map_width);
    glUniform1i(glGetUniformLocation(shader_program, "map_height"), map_height);
    glUseProgram(0);
    /* tests to see if shader is working properly
    int success;
    char infoLog[512];
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(shader_program, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<
                  infoLog << std::endl;
    }
    if(success){
        std::cout << "shader program successfully compiled";
    }
    */

    GLuint VBO, instanceVBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &instanceVBO);
    glBindVertexArray(VAO);
    // vertices buffer
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), nullptr);
    glEnableVertexAttribArray(0);
    // Instance buffer for tile_offset and is alive data
        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, long(tile_offset.size() * 2 * sizeof(GLfloat) + tile_is_alive.size() * sizeof(GLuint)), tile_offset.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), nullptr);
    // is alive bool, 1 is alive 0 is dead
    GLintptr translation_offset = long(tile_offset.size() * 2 * sizeof(GLfloat));
    glBufferSubData(GL_ARRAY_BUFFER, translation_offset, long(tile_is_alive.size() * sizeof(GLuint)), tile_is_alive.data());
    glVertexAttribPointer(2, 1, GL_UNSIGNED_INT, GL_FALSE, sizeof(GLuint), (GLvoid*)translation_offset);

    glVertexAttribDivisor(1, 1);
    glVertexAttribDivisor(2, 1);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);


    float test_index = 0;
    bool is_paused = true;
    int is_paused_uniform;
    float counter = 1;

    double xpos, ypos;

    while(!glfwWindowShouldClose(window)){
        glfwSetWindowSizeCallback(window, window_size_callback);
        glfwGetCursorPos(window, &xpos, &ypos);
        //std::cout << "Xpos: " << xpos << ", Ypos: " << ypos << "\n";

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader_program);
        glBindVertexArray(VAO);
        glBufferSubData(GL_ARRAY_BUFFER, translation_offset, long(tile_is_alive.size() * sizeof(GLuint)), tile_is_alive.data());
        glDrawArraysInstanced(GL_TRIANGLES, 0, 18, int(tile_offset.size()));
        glBindVertexArray(0);

        counter += 0.001;
        if (counter > 2){
            counter = 2;
        }
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
            if (is_paused){
                if (counter > 1){
                    is_paused = false;
                    counter = 0;
                }
            }
            else {
                if (counter > 1) {
                    is_paused = true;
                    counter = 0;
                }
            }
        }
        if (is_paused){
            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
                //std::cout << "You just clicked!\n";
                click_input(tile_is_alive, xpos, ypos, map_width, map_height, window_width, window_height);
            }
            if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS){
                std::cout << "Clearing Map...\n";
                clear_map(tile_is_alive);
            }
        }


        test_index += 0.005;
        if (test_index > 1){
            test_index = 0;
            if (!is_paused){
                update_game(tile_is_alive, map_width, map_height);
            }
        }

        if (is_paused)  {is_paused_uniform = 1;}
        else            {is_paused_uniform = 0;}
        glUniform1i(glGetUniformLocation(shader_program, "is_paused"), is_paused_uniform);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

   glfwTerminate();
   return 0;
}
