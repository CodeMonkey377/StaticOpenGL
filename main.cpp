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

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    int map_width = 50;
    int map_height = 50;

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
    std::vector<glm::vec2> translations;
    int index_x = 0;
    int index_y = 0;
    for (int i = 0; i < map_height; i++){
        for (int j = 0; j < map_width; j++){
            translations.emplace_back(index_x, index_y);
            index_x += 2;
        }
        index_x = 0;
        index_y += 2;
    }
    // vector that conveys weather a given tile is alive or dead. Runs bottom left to top right
    std::vector<GLuint> tile_is_alive(map_width * map_height, 0);
    tile_is_alive[1] = 1;
    tile_is_alive[51] = 1;
    tile_is_alive[101] = 1;

    GLFWwindow* window = glfwCreateWindow(1500, 1500, "Static OpenGL", nullptr, nullptr);
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
   glViewport(0, 0, 1500, 1500);

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
    // Instance buffer for translations and is alive data
        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, long(translations.size() * 2 * sizeof(GLfloat) + tile_is_alive.size() * sizeof(GLuint)), translations.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), nullptr);
    // is alive bool, 1 is alive 0 is dead
    GLintptr translation_offset = long(translations.size() * 2 * sizeof(GLfloat));
    glBufferSubData(GL_ARRAY_BUFFER, translation_offset, long(tile_is_alive.size() * sizeof(GLuint)), tile_is_alive.data());
    glVertexAttribPointer(2, 1, GL_UNSIGNED_INT, GL_FALSE, sizeof(GLuint), (GLvoid*)translation_offset);

    glVertexAttribDivisor(1, 1);
    glVertexAttribDivisor(2, 1);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);


    float test_index = 0;
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader_program);

        glBindVertexArray(VAO);
        glBufferSubData(GL_ARRAY_BUFFER, translation_offset, long(tile_is_alive.size() * sizeof(GLuint)), tile_is_alive.data());
        glDrawArraysInstanced(GL_TRIANGLES, 0, 18, int(translations.size()));
        glBindVertexArray(0);

        test_index += 0.001;
        if (test_index > 1){
            test_index = 0;
            update_game(tile_is_alive, map_width, map_height);
        }

        glfwSwapBuffers(window);
    }

   glfwTerminate();
   return 0;
}
