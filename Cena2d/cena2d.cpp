#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

// Funções de callback
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// Função para desenhar elipses
void drawEllipse(float cx, float cy, float rx, float ry, int numSegments, float r, float g, float b) {
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(r, g, b); // Cor da elipse
    for (int i = 0; i <= numSegments; i++) {
        float angle = i * 2.0f * 3.1415926f / numSegments;
        float x = cosf(angle) * rx;
        float y = sinf(angle) * ry;
        glVertex2f(cx + x, cy + y);
    }
    glEnd();
}

// Função para desenhar triângulos
void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, float r, float g, float b) {
    glBegin(GL_TRIANGLES);
    glColor3f(r, g, b); // Cor do triângulo
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glVertex2f(x3, y3);
    glEnd();
}

// Função para desenhar o peixe
void drawFish() {
    // Corpo do peixe
    drawEllipse(0.6f, -0.2f, 0.2f, 0.1f, 100, 1.0f, 0.5f, 0.0f); // Corpo laranja

    // Nadadeiras do peixe
    drawTriangle(0.4f, -0.1f, 0.3f, 0.0f, 0.3f, -0.2f, 1.0f, 0.5f, 0.8f);  // Nadadeira superior
    drawTriangle(0.4f, -0.3f, 0.3f, -0.2f, 0.3f, -0.4f, 1.0f, 0.5f, 0.8f);  // Nadadeira inferior

    // Olho do peixe
    drawEllipse(0.65f, -0.15f, 0.03f, 0.03f, 50, 1.0f, 1.0f, 1.0f);  // Olho branco
    drawEllipse(0.67f, -0.15f, 0.015f, 0.015f, 50, 0.0f, 0.0f, 0.0f); // Pupila preta
}

// Função para desenhar o gato
void drawCat() {
    // Corpo do gato
    drawEllipse(-0.6f, -0.2f, 0.2f, 0.3f, 100, 0.6f, 0.6f, 0.6f); // Corpo cinza
    drawEllipse(-0.6f, 0.1f, 0.15f, 0.15f, 100, 0.6f, 0.6f, 0.6f); // Cabeça cinza

    // Orelhas do gato (ajustadas)
    drawTriangle(-0.7f, 0.2f, -0.65f, 0.35f, -0.6f, 0.2f, 0.6f, 0.6f, 0.6f); // Orelha esquerda
    drawTriangle(-0.5f, 0.2f, -0.55f, 0.35f, -0.6f, 0.2f, 0.6f, 0.6f, 0.6f); // Orelha direita

    // Olhos do gato
    drawEllipse(-0.65f, 0.15f, 0.03f, 0.03f, 50, 1.0f, 1.0f, 1.0f); // Olho esquerdo
    drawEllipse(-0.55f, 0.15f, 0.03f, 0.03f, 50, 1.0f, 1.0f, 1.0f); // Olho direito
    drawEllipse(-0.65f, 0.15f, 0.015f, 0.015f, 50, 0.0f, 0.0f, 0.0f); // Pupila esquerda
    drawEllipse(-0.55f, 0.15f, 0.015f, 0.015f, 50, 0.0f, 0.0f, 0.0f); // Pupila direita

    // Focinho do gato
    drawTriangle(-0.6f, 0.1f, -0.58f, 0.1f, -0.59f, 0.05f, 0.0f, 0.0f, 0.0f); // Focinho preto
}

// Função para desenhar o arranhador de gato
void drawScratchingPost() {
    // Base do arranhador
    glBegin(GL_QUADS);
    glColor3f(1.0f, 0.5f, 0.5f); // Cor rosa
    glVertex2f(-0.1f, -0.6f);    // Canto inferior esquerdo
    glVertex2f(0.1f, -0.6f);     // Canto inferior direito
    glVertex2f(0.1f, -0.8f);     // Canto superior direito
    glVertex2f(-0.1f, -0.8f);    // Canto superior esquerdo
    glEnd();

    // Poste do arranhador
    glBegin(GL_QUADS);
    glColor3f(0.96f, 0.87f, 0.70f); // Cor bege
    glVertex2f(-0.05f, -0.6f);
    glVertex2f(0.05f, -0.6f);
    glVertex2f(0.05f, 0.5f);
    glVertex2f(-0.05f, 0.5f);
    glEnd();

    // Bola no topo do arranhador
    drawEllipse(0.0f, 0.6f, 0.1f, 0.1f, 100, 1.0f, 0.0f, 0.0f); // Vermelho
}

int main() {
    // Inicializar o GLFW
    if (!glfwInit()) {
        std::cerr << "Falha ao inicializar o GLFW" << std::endl;
        return -1;
    }

    // Criar a janela GLFW
    GLFWwindow* window = glfwCreateWindow(800, 600, "Cena Completa - Gato, Peixe e Arranhador", NULL, NULL);
    if (!window) {
        std::cerr << "Falha ao criar a janela GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Inicializar GLEW
    if (glewInit() != GLEW_OK) {
        std::cerr << "Falha ao inicializar o GLEW" << std::endl;
        return -1;
    }

    // Definir a função de redimensionamento da janela
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Loop de renderização
    while (!glfwWindowShouldClose(window)) {
        // Processar a entrada do usuário
        processInput(window);

        // Limpar o buffer de cor
        glClearColor(0.5f, 0.8f, 1.0f, 1.0f); // Cor do fundo (azul claro)
        glClear(GL_COLOR_BUFFER_BIT);

        // Desenhar os objetos na cena
        drawFish();           // Desenha o peixe
        drawCat();            // Desenha o gato
        drawScratchingPost(); // Desenha o arranhador

        // Trocar os buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Encerrar o GLFW
    glfwTerminate();
    return 0;
}

// Processar entrada do teclado
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// Função de callback para redimensionamento de janela
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

