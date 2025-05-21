#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>

// Função para criar um triângulo com coordenadas arbitrárias e retornar o VAO
GLuint createTriangle(float x0, float y0, float x1, float y1, float x2, float y2) {
    float vertices[] = {
        x0, y0,
        x1, y1,
        x2, y2
    };

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Define como os dados de vértices são interpretados pelo shader
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return VAO;
}

// Vertex Shader simples - posiciona os vértices na tela
const char* vertexShaderSource = "#version 330 core\n"
    "layout(location = 0) in vec2 aPos;\n"
    "void main() {\n"
    "    gl_Position = vec4(aPos, 0.0, 1.0);\n"
    "}";

// Fragment Shader simples - define cor fixa para os triângulos
const char* fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main() {\n"
    "    FragColor = vec4(0.4, 0.8, 1.0, 1.0);\n"
    "}";

// Função para compilar os shaders e linkar o programa de shader
GLuint compileShaderProgram() {
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

int main() {
    // Inicializa a GLFW e configura a versão do OpenGL
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Cria a janela
    GLFWwindow* window = glfwCreateWindow(800, 600, "Parte 1 - Triangulos", NULL, NULL);
    glfwMakeContextCurrent(window);
    gladLoadGL(); // Carrega funções da OpenGL via GLAD

    // Compila os shaders e cria o programa
    GLuint shaderProgram = compileShaderProgram();

    // Cria 5 triângulos com posições diferentes usando a função criada
    std::vector<GLuint> VAOs;
    VAOs.push_back(createTriangle(-0.8f, -0.5f, -0.6f, -0.5f, -0.7f, -0.3f));
    VAOs.push_back(createTriangle(-0.4f, -0.5f, -0.2f, -0.5f, -0.3f, -0.3f));
    VAOs.push_back(createTriangle(0.0f, -0.5f, 0.2f, -0.5f, 0.1f, -0.3f));
    VAOs.push_back(createTriangle(0.4f, -0.5f, 0.6f, -0.5f, 0.5f, -0.3f));
    VAOs.push_back(createTriangle(-0.2f, 0.0f, 0.0f, 0.0f, -0.1f, 0.2f));

    // Loop principal de renderização
    while (!glfwWindowShouldClose(window)) {
        // Limpa a tela com cor de fundo escura
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Usa o shader program compilado
        glUseProgram(shaderProgram);

        // Renderiza cada triângulo armazenado
        for (auto vao : VAOs) {
            glBindVertexArray(vao);
            glDrawArrays(GL_TRIANGLES, 0, 3);
        }

        glfwSwapBuffers(window); // Exibe o framebuffer
        glfwPollEvents(); // Lida com eventos (como fechar janela)
    }

    // Finaliza o contexto GLFW
    glfwTerminate();
    return 0;
}
