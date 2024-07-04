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

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Two Triangles that make up square
    GLfloat vertices[12] = {
            1.0f, 1.0f,
            -1.0f, 1.0f,
            -1.0f, -1.0f,

            -1.0f, -1.0f,
            1.0f, -1.0f,
            1.0f, 1.0,
    };

    glm::vec2 translations[1] = {
            glm::vec2 (2, 0)
    };

    std::cout << "\n" << translations[0].x << "," << translations[0].y << "\n";
    std::cout << "Size of translations: " << sizeof(translations) << "\n";
    std::cout << "Size of 2 GL floats: " << sizeof(GLfloat) * 2 << "\n";

    struct tile{
        // represents corner of tile or maybe offset
        std::vector<glm::vec2> position;

        /// Value of neighbors, need to figure out how pos will work first

        // will go to fragment shader when drawing I am guessing
        bool is_on;
    };
    int map_width;
    int map_height;

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
   std::cout << "Loaded Open GL\n";
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

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), nullptr);
    glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(translations), &translations[0], GL_STATIC_DRAW);
    GLintptr vertex_position_offset = 0;
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)vertex_position_offset);
    glVertexAttribDivisor(1, 1);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);



    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader_program);

        glBindVertexArray(VAO);
        glDrawArraysInstanced(GL_TRIANGLES, 0, 18, 2);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

   glfwTerminate();
   return 0;
}
