#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// Vertex Shader
const char* vertexShaderSource = R"(
#version 400
layout (location = 0) in vec3 position;
uniform mat4 model;
void main() {
    gl_Position = model * vec4(position, 1.0);
}
)";

// Fragment Shader
const char* fragmentShaderSource = R"(
#version 400
uniform vec4 inputColor;
out vec4 color;
void main() {
    color = inputColor;
}
)";

// Função de checagem de shader
void checkShader(GLuint shader, const char* type) {
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "Erro " << type << ": " << infoLog << std::endl;
    }
}

int main() {
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(800, 600, "5 Triângulos Coloridos", NULL, NULL);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glViewport(0, 0, 800, 600);

    // Dados do triângulo base
    float vertices[] = {
        0.0f,  0.1f, 0.0f,
       -0.1f, -0.1f, 0.0f,
        0.1f, -0.1f, 0.0f
    };

    // VAO e VBO
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Shaders
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    checkShader(vertexShader, "VERTEX");

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    checkShader(fragmentShader, "FRAGMENT");

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glUseProgram(shaderProgram);
    GLint colorLoc = glGetUniformLocation(shaderProgram, "inputColor");
    GLint modelLoc = glGetUniformLocation(shaderProgram, "model");

    // Cores dos triângulos
    float colors[5][3] = {
        {1.0f, 0.0f, 0.0f},  // Vermelho
        {0.0f, 1.0f, 0.0f},  // Verde
        {0.0f, 0.0f, 1.0f},  // Azul
        {1.0f, 1.0f, 0.0f},  // Amarelo
        {1.0f, 0.0f, 1.0f}   // Magenta
    };

    // Translações dos triângulos
    float offsets[5] = {-0.6f, -0.3f, 0.0f, 0.3f, 0.6f};

    // Loop principal
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO);
        glUseProgram(shaderProgram);

        for (int i = 0; i < 5; ++i) {
            // Define cor atual
            glUniform4f(colorLoc, colors[i][0], colors[i][1], colors[i][2], 1.0f);

            // Matriz de transformação
            float offset = offsets[i];
            float model[16] = {
                1.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 0.0f,
                offset, 0.0f, 0.0f, 1.0f
            };
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model);

            // Desenha triângulo
            glDrawArrays(GL_TRIANGLES, 0, 3);
        }

        glfwSwapBuffers(window);
    }

    // Limpeza
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();

    return 0;
}