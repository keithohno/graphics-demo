#include <iostream>
#include <fstream>
#include <sstream>

// GLEW 
#define GLEW_STATIC 
#include <GL/glew.h> 

// GLFW 
#include <GLFW/glfw3.h> 


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

    // vertex data
    float outer_vertices[] = {
            0.0, 0.0, 0, 1.0, 1.0, 1.0,
            0.0, -0.9, 0, 1.0, 0.0, 0.0,
            -0.7, -0.7, 0, 0.625, 0.375, 0.7,
            -0.9, 0.0, 0, 0.25, 0.75, 0.7,
            -0.7, 0.7, 0, 0.0, .875, 0.125,
            0.0, 0.9, 0, 0.0, 0.5, 0.5,
            0.7, 0.7, 0, 0.0, 0.125, 0.875,
            0.9, 0.0, 0, 0.25, 0.0, 0.75,
            0.7, -0.7, 0, 0.625, 0.0, 0.375,
    };
    unsigned int indices[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 1 };

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

    // read shader code from files
    std::ifstream frag_ifs("C:\\Users\\keith\\source\\repos\\graphics-demo\\Debug\\shaders\\frag_shader.glsl");
    std::ifstream vert_ifs("C:\\Users\\keith\\source\\repos\\graphics-demo\\Debug\\shaders\\vert_shader.glsl");

    // check that shader files were found
    if (!frag_ifs || !vert_ifs) {
        std::cout << "frag shader found: " << (bool)frag_ifs << std::endl;
        std::cout << "vert shader found: " << (bool)vert_ifs << std::endl;
        return -1;
    }

    // read shader code to strings
    std::stringstream buf;
    buf << frag_ifs.rdbuf();
    std::string frag_shader_str = buf.str();
    char const* frag_shader_source = frag_shader_str.c_str();
    buf.str("");
    buf.clear();
    buf << vert_ifs.rdbuf();
    std::string vert_shader_str = buf.str();
    char const* vert_shader_source = vert_shader_str.c_str();

    // compile shaders
    unsigned int frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader, 1, &frag_shader_source, NULL);
    glCompileShader(frag_shader);
    unsigned int vert_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert_shader, 1, &vert_shader_source, NULL);
    glCompileShader(vert_shader);

    // create shader program object
    unsigned int shader_program = glCreateProgram();
    glAttachShader(shader_program, frag_shader);
    glAttachShader(shader_program, vert_shader);
    glLinkProgram(shader_program);

    // check errors
    int success;
    char log[512];
    glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vert_shader, 512, NULL, log);
        std::cout << "[ERROR] Failed to compile frag shader: " << log << std::endl;
    }
    glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(frag_shader, 512, NULL, log);
        std::cout << "[ERROR] Failed to compile vert shader: " << log << std::endl;
    }

    // delete old vert/frag shaders
    glDeleteShader(frag_shader);
    glDeleteShader(vert_shader);

    // use shaders
    glUseProgram(shader_program);

    // SHADER END

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

        // copy data to buffers
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(outer_vertices), outer_vertices, GL_STATIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        
        // bind vao and draw
        glBindVertexArray(VAO);
        glBindVertexBuffer(0, VBO, 0, 6 * sizeof(float));
        glDrawElements(GL_TRIANGLE_FAN, 10, GL_UNSIGNED_INT, 0);

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
