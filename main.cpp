#define _USE_MATH_DEFINES

#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "shader.h"

const int NUM_VERTS = 50;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main(int argc, char* argv[]) {

    // glfw setup (opengl 4.6)
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGLDemo", nullptr, nullptr);
    if (!window) {
        std::cout << "[ERROR] Failed to create GLFW window!" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    // glew setup
    glewExperimental = GL_TRUE;
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    // outer vertices calculation
    float outer_vertices[6 * (NUM_VERTS + 1)] = { 0 };
    for (int i = 1; i < NUM_VERTS + 1; i++) {
        // positioned around a circle
        float theta = 2 * M_PI * (i - 1) / NUM_VERTS;
        outer_vertices[6 * i] = sin(theta) * 0.9;
        outer_vertices[6 * i + 1] = cos(theta) * 0.9;
        // color progreses from r -> g -> b depending on theta
        if (theta < 2 * M_PI / 3) {
            float part = theta / (2 * M_PI / 3);
            outer_vertices[6 * i + 3] = 1 - theta / (2 * M_PI / 3);
            outer_vertices[6 * i + 4] = theta / (2 * M_PI / 3);
        }
        else if (theta < 4 * M_PI / 3) {
            float part = (theta - 2 * M_PI / 3) / (2 * M_PI / 3);
            outer_vertices[6 * i + 4] = 1 - part;
            outer_vertices[6 * i + 5] = part;
        }
        else {
            float part = (theta - 4 * M_PI / 3) / (2 * M_PI / 3);
            outer_vertices[6 * i + 5] = 1 - part;
            outer_vertices[6 * i + 3] = part;
        }
    }
    outer_vertices[3] = 1.0;
    outer_vertices[4] = 1.0;
    outer_vertices[5] = 1.0;

    // inner vertices calculation
    float inner_vertices[6 * (NUM_VERTS + 1)] = { 0 };
    for (int i = 1; i < NUM_VERTS + 1; i++) {
        // same as outer vertices but smaller circle
        float theta = 2 * M_PI * (i - 1) / NUM_VERTS;
        inner_vertices[6 * i] = sin(theta) * 0.4;
        inner_vertices[6 * i + 1] = cos(theta) * 0.4;
    }

    // indices for EBO
    // [ 0, 1, 2, .. NUM_VERTS, 1 ] to match gl_triangle_fan
    unsigned int indices[NUM_VERTS + 2] = {0};
    for (int i = 1; i < NUM_VERTS + 1; i++) {
        indices[i] = i;
    }
    indices[NUM_VERTS + 1] = 1;

    // generate buffers
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // bind buffers
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    // specify VBO interpretation
    glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float));
    glVertexAttribBinding(0, 0);
    glVertexAttribBinding(1, 0);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // SHADER START
    Shader shader = Shader("Debug\\shaders\\vert_shader.glsl", "Debug\\shaders\\frag_shader.glsl");
    shader.use();

    glViewport(0, 0, 800, 600);

    // render loop
    while (!glfwWindowShouldClose(window)) {
        // close on ESC
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }

        // clear to black
        glClearColor(0.0f, 0.0f, 0.0f, 0.1f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // bind vao and set
        glBindVertexArray(VAO);
        glBindVertexBuffer(0, VBO, 0, 6 * sizeof(float));

        // set ebo index data
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // buffer and draw outer circle
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(outer_vertices), outer_vertices, GL_STATIC_DRAW);
        glDrawElements(GL_TRIANGLE_FAN, NUM_VERTS + 2, GL_UNSIGNED_INT, 0);

        // buffer and draw inner circle (VBO already bound)
        glBufferData(GL_ARRAY_BUFFER, sizeof(inner_vertices), inner_vertices, GL_STATIC_DRAW);
        glDrawElements(GL_TRIANGLE_FAN, NUM_VERTS + 2, GL_UNSIGNED_INT, 0);

        // poll events and swap buffers
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}


void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}
