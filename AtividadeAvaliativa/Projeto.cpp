#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <cstdlib>
#include <ctime>

struct Vertex {
    float x, y;
};

struct Triangle {
    Vertex vertices[3];
    float r, g, b;
};

std::vector<Vertex> tempVertices;
std::vector<Triangle> triangles;

unsigned int VAO, VBO;

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        int width, height;
        glfwGetWindowSize(window, &width, &height);

        Vertex v;
        v.x = static_cast<float>(xpos);
        v.y = static_cast<float>(height - ypos); // Invertendo o Y para corresponder às coordenadas OpenGL

        tempVertices.push_back(v);

        if (tempVertices.size() == 3) {
            Triangle t;
            for (int i = 0; i < 3; i++) {
                t.vertices[i] = tempVertices[i];
            }
            t.r = (float)(rand() % 100) / 100.0f;
            t.g = (float)(rand() % 100) / 100.0f;
            t.b = (float)(rand() % 100) / 100.0f;

            triangles.push_back(t);
            tempVertices.clear();
        }
    }
}

void setupTriangle() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
}

void renderTriangles(int width, int height) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, 0, height, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    for (const auto& t : triangles) {
        float vertices[6] = {
            t.vertices[0].x, t.vertices[0].y,
            t.vertices[1].x, t.vertices[1].y,
            t.vertices[2].x, t.vertices[2].y
        };

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glColor3f(t.r, t.g, t.b);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
}

int main() {
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(800, 600, "Triangles Click", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    gladLoadGL();

    srand(time(0));

    setupTriangle();

    glfwSetMouseButtonCallback(window, mouse_button_callback);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        int width, height;
        glfwGetWindowSize(window, &width, &height);

        renderTriangles(width, height);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}
