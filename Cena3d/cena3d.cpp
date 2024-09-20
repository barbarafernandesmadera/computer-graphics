#include <GL/glew.h>    // Sempre inclua o GLEW primeiro
#include <GLFW/glfw3.h> // GLFW pode ser incluído depois do GLEW
#include <GL/glut.h>    // GLUT, se necessário, vem depois do GLEW (opcional)
#include <GL/glext.h>   // Inclua glext.h apenas se necessário e após glew.h

#include <cmath> // Para usar funções matemáticas como cos() e sin()
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

//Caixa de areia

// Definições para o círculo (cabeça)
const int numCircleVertices = 20;
float circleRadius = 0.16f; // Aumentei o raio do círculo para que a cabeça do gato fique maior
float circleCenterX = 0.0f;
float circleCenterY = 0.65f; // Movi um pouco a posição para acomodar o novo tamanho

// Variável global para o shader program
unsigned int shaderProgram;

// Vértices da caixa de areia tridimensional + cubo colado no topo
float verticesCubo[] = {
    // Posições           // Cores (verdes para o paralelepípedo, rosas para o cubo)
    // Base do paralelepípedo (no chão) - tons de verde
    -0.5f, 0.0f, -0.5f,   0.0f, 0.8f, 0.0f, // Vértice inferior esquerdo
     0.5f, 0.0f, -0.5f,   0.2f, 0.6f, 0.2f, // Vértice inferior direito
     0.5f, 0.0f,  0.5f,   0.4f, 0.9f, 0.4f, // Vértice superior direito
    -0.5f, 0.0f,  0.5f,   0.1f, 0.7f, 0.1f, // Vértice superior esquerdo

    // Parede frontal - tons de verde
    -0.5f, 0.0f, -0.5f,   0.0f, 0.8f, 0.0f, // Vértice inferior esquerdo
     0.5f, 0.0f, -0.5f,   0.2f, 0.6f, 0.2f, // Vértice inferior direito
     0.5f, 0.3f, -0.5f,   0.4f, 0.9f, 0.4f, // Vértice superior direito
    -0.5f, 0.3f, -0.5f,   0.1f, 0.7f, 0.1f, // Vértice superior esquerdo

    // Parede traseira - tons de verde
    -0.5f, 0.0f,  0.5f,   0.1f, 0.9f, 0.1f, // Vértice inferior esquerdo
     0.5f, 0.0f,  0.5f,   0.3f, 0.7f, 0.3f, // Vértice inferior direito
     0.5f, 0.3f,  0.5f,   0.2f, 0.8f, 0.2f, // Vértice superior direito
    -0.5f, 0.3f,  0.5f,   0.0f, 0.6f, 0.0f, // Vértice superior esquerdo

    // Parede esquerda - tons de verde
    -0.5f, 0.0f, -0.5f,   0.2f, 0.8f, 0.2f, // Vértice inferior esquerdo
    -0.5f, 0.0f,  0.5f,   0.3f, 0.9f, 0.3f, // Vértice inferior direito
    -0.5f, 0.3f,  0.5f,   0.1f, 0.6f, 0.1f, // Vértice superior direito
    -0.5f, 0.3f, -0.5f,   0.2f, 0.7f, 0.2f, // Vértice superior esquerdo

    // Parede direita - tons de verde
     0.5f, 0.0f, -0.5f,   0.0f, 0.8f, 0.0f, // Vértice inferior esquerdo
     0.5f, 0.0f,  0.5f,   0.2f, 0.6f, 0.2f, // Vértice inferior direito
     0.5f, 0.3f,  0.5f,   0.4f, 0.9f, 0.4f, // Vértice superior direito
     0.5f, 0.3f, -0.5f,   0.1f, 0.7f, 0.1f, // Vértice superior esquerdo

    // Vértices do cubo colado no topo do paralelepípedo - tons de rosa
    -0.25f, 0.3f, -0.25f,  1.0f, 0.5f, 0.6f, // Vértice inferior esquerdo da base
     0.25f, 0.3f,  0.25f,  1.0f, 0.3f, 0.7f, // Vértice superior direito da base
    -0.25f, 0.3f,  0.25f,  1.0f, 0.4f, 0.5f, // Vértice superior esquerdo da base

    -0.25f, 1.0f, -0.25f,  1.0f, 0.5f, 0.7f, // Vértice inferior esquerdo do topo
     0.25f, 1.0f, -0.25f,  1.0f, 0.4f, 0.6f, // Vértice inferior direito do topo
     0.25f, 1.0f,  0.25f,  1.0f, 0.3f, 0.7f, // Vértice superior direito do topo
    -0.25f, 1.0f,  0.25f,  1.0f, 0.6f, 0.5f  // Vértice superior esquerdo do topo
};

// Índices para desenhar os quads (pares de triângulos) que formam a caixa e o cubo
unsigned int indicesCubo[] = {
    // Base do paralelepípedo
    0, 1, 2,
    2, 3, 0,

    // Parede frontal
    4, 5, 6,
    6, 7, 4,

    // Parede traseira
    8, 9, 10,
    10, 11, 8,

    // Parede esquerda
    12, 13, 14,
    14, 15, 12,

    // Parede direita
    16, 17, 18,
    18, 19, 16,

    // Índices para o cubo
    // Base
    20, 21, 22,
    22, 23, 20,

    // Topo
    24, 25, 26,
    26, 27, 24,

    // Parede frontal
    20, 21, 25,
    25, 24, 20,

    // Parede traseira
    23, 22, 26,
    26, 27, 23,

    // Parede esquerda
    20, 23, 27,
    27, 24, 20,

    // Parede direita
    21, 22, 26,
    26, 25, 21
};

// Definições para o círculo e orelhas (cabeça do gato na parede frontal do cubo)
float catHeadVertices[3 * (numCircleVertices + 6)]; // Para as orelhas e o círculo

void initCatHeadVertices() {
    // Orelhas (dois triângulos, ajustados para serem maiores)
    catHeadVertices[0] = -0.15f;  catHeadVertices[1] = 0.85f;  catHeadVertices[2] = -0.25f; // Orelha esquerda (topo)
    catHeadVertices[3] = -0.2f;   catHeadVertices[4] = 0.7f;   catHeadVertices[5] = -0.25f; // Orelha esquerda (base esquerda)
    catHeadVertices[6] = -0.1f;   catHeadVertices[7] = 0.7f;   catHeadVertices[8] = -0.25f; // Orelha esquerda (base direita

    catHeadVertices[9]  = 0.15f;  catHeadVertices[10] = 0.85f; catHeadVertices[11] = -0.25f; // Orelha direita (topo)
    catHeadVertices[12] = 0.1f;   catHeadVertices[13] = 0.7f;  catHeadVertices[14] = -0.25f; // Orelha direita (base esquerda)
    catHeadVertices[15] = 0.2f;   catHeadVertices[16] = 0.7f;  catHeadVertices[17] = -0.25f; // Orelha direita (base direita)

    // Círculo (cabeça)
    for (int i = 0; i < numCircleVertices; ++i) {
        float angle = 2.0f * M_PI * float(i) / float(numCircleVertices);
        float x = circleCenterX + circleRadius * cos(angle);
        float y = circleCenterY + circleRadius * sin(angle);
        
        catHeadVertices[18 + 3 * i]     = x;
        catHeadVertices[18 + 3 * i + 1] = y;
        catHeadVertices[18 + 3 * i + 2] = -0.25f; // Z fixo
    }
}

// Índices para desenhar orelhas e o círculo
unsigned int catHeadIndices[3 * numCircleVertices + 12]; // Para o círculo e orelhas

void initCatHeadIndices() {
    // Orelhas
    catHeadIndices[0] = 0; catHeadIndices[1] = 1; catHeadIndices[2] = 2; // Orelha esquerda
    catHeadIndices[3] = 3; catHeadIndices[4] = 4; catHeadIndices[5] = 5; // Orelha direita

    // Círculo (triângulos que formam o círculo)
    for (int i = 0; i < numCircleVertices; ++i) {
        int nextIndex = (i + 1) % numCircleVertices;
        catHeadIndices[6 + 3 * i]     = 6 + i;      // Vértice atual do círculo
        catHeadIndices[6 + 3 * i + 1] = 6 + nextIndex; // Próximo vértice do círculo
        catHeadIndices[6 + 3 * i + 2] = 6;          // Centro do círculo
    }
}

//Rede

// Variáveis de controle de transformações para a rede
float angleX = 0.0f;
float angleY = 0.0f;
float scale = 1.0f;
float translateX = 0.0f;
float translateY = 0.0f;
bool wireframe = false;  // Alternar entre wireframe e preenchido


// Função para desenhar a rede (almofada)
void drawPillowWithRoundedEdges() {
    glColor3f(0.8, 0.8, 0.8);  // Cor cinza claro

    // Desenhar a parte principal da almofada como um paralelepípedo
    glPushMatrix();
    glTranslatef(0.0, 0.3, 0.0);  // Elevar a almofada acima da estrutura de madeira
    glScalef(2.6, 0.3, 1.0);  // Escalando o cubo para alongar a "almofada"
    glutSolidCube(1.0);
    glPopMatrix();

    // Tamanho das esferas nas bordas
    float radius = 0.2f;

    // Desenhar esferas nas quatro extremidades superiores
    glPushMatrix();
    glTranslatef(-1.3, 0.45, -0.5);  // Ajuste para a nova altura da almofada
    glutSolidSphere(radius, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1.3, 0.45, -0.5);  // Ajuste para a nova altura da almofada
    glutSolidSphere(radius, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-1.3, 0.45, 0.5);  // Ajuste para a nova altura da almofada
    glutSolidSphere(radius, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1.3, 0.45, 0.5);  // Ajuste para a nova altura da almofada
    glutSolidSphere(radius, 20, 20);
    glPopMatrix();
}

// Função para desenhar o apoio da rede (estrutura de madeira)
void drawHammockSupport() {
    glColor3f(0.6, 0.4, 0.2);  // Marrom claro para simular a madeira

    // Desenha a base (dois pés)
    glPushMatrix();
    glTranslatef(0.0, -0.5, 0.0);  // Ajustar para que os pés fiquem mais abaixo da estrutura
    glScalef(0.2, 0.05, 1.5);  // Base alongada
    glutSolidCube(1.0);
    glPopMatrix();

    // Desenha os suportes laterais
    glPushMatrix();
    glTranslatef(-0.6, 0.0, 0.0);  // Lado esquerdo, aproximado para o centro
    glRotatef(-45, 0.0, 0.0, 1.0);  // Inclina o suporte
    glScalef(0.2, 1.0, 0.2);  // Dimensões do suporte
    glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.6, 0.0, 0.0);  // Lado direito, aproximado para o centro
    glRotatef(45, 0.0, 0.0, 1.0);  // Inclina o suporte
    glScalef(0.2, 1.0, 0.2);  // Dimensões do suporte
    glutSolidCube(1.0);
    glPopMatrix();
}

// Função para desenhar a estrutura que une as laterais
void drawCentralSupport() {
    glColor3f(0.6, 0.4, 0.2);  // Cor marrom para a barra central

    // Desenha uma barra central conectando as duas laterais e posicionada no chão
    glPushMatrix();
    glTranslatef(0.0, -0.4, 0.0);  // Posiciona a barra no chão
    glScalef(1.6, 0.2, 0.2);  // Escalando para alongar a barra no eixo X
    glutSolidCube(1.0);
    glPopMatrix();
}


// Função de inicialização
void init() {
    glClearColor(1.0, 1.0, 5.0, 1.0);  // Fundo amarelo claro
    glEnable(GL_DEPTH_TEST);  // Habilita o teste de profundidade
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 1.0, 1.0, 500.0);  // Projeção perspectiva
    glMatrixMode(GL_MODELVIEW);
}


float rotationAngle = 0.0f;
// Função para desenhar a caixa de areia
void drawSandboxAndCat(unsigned int shaderProgram) {
    // Usar o shader program
    glUseProgram(shaderProgram);

    // Criar a matriz de modelo (aplicando rotação)
    glm::mat4 model = glm::mat4(1.0f);  // Inicializa com matriz identidade
    model = glm::rotate(model, glm::radians(rotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));  // Rotação no eixo Y

    // Criar a matriz de visualização (câmera)
    glm::mat4 view = glm::mat4(1.0f);  // Inicializa com matriz identidade
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));  // Move a câmera para trás

    // Criar a matriz de projeção (perspectiva)
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    // Obter os locais das matrizes no shader
    int modelLoc = glGetUniformLocation(shaderProgram, "model");
    int viewLoc = glGetUniformLocation(shaderProgram, "view");
    int projLoc = glGetUniformLocation(shaderProgram, "projection");

    // Enviar as matrizes para o shader
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    //Desenha objetos da caixa de areia do gato
    glColor3f(0.1f, 0.7f, 0.1f); // Caixa verde
    glPushMatrix();
    glScalef(1.0, 0.3, 1.0);
    glutSolidCube(1.0);
    glPopMatrix();

    // Desenha o cubo rosa (casa do gato)
    glColor3f(1.0f, 0.4f, 0.6f);
    glPushMatrix();
    glTranslatef(0.0, 0.35, 0.0);  // Posiciona acima da base
    glScalef(0.5, 0.7, 0.5);
    glutSolidCube(1.0);
    glPopMatrix();

    // Desenha a cabeça do gato (esfera)
    glColor3f(0.0f, 0.0f, 0.0f);
    glPushMatrix();
    glTranslatef(0.0, 0.35, -0.15);  // Coloca na frente do cubo
    glutSolidSphere(0.2, 20, 20);
    glPopMatrix();
}


// Função de exibição 
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0.0, 1.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    if (wireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    glTranslatef(translateX, translateY, 0.0);
    glRotatef(angleX, 1.0, 0.0, 0.0);
    glRotatef(angleY, 0.0, 1.0, 0.0);
    glScalef(scale, scale, scale);

    // Desenha a rede de gato
    glPushMatrix();
    glTranslatef(-1.5, 0.0, 0.0);
    drawHammockSupport();
    drawCentralSupport();
    drawPillowWithRoundedEdges();
    glPopMatrix();

    // Desenha a caixa de areia com gato
    glPushMatrix();
    glTranslatef(1.5, 0.0, 0.0);
    drawSandboxAndCat(shaderProgram);
    glPopMatrix();

    glutSwapBuffers();
}





// Função para atualizar a rotação da cena
void idle() {
    rotationAngle += 0.5f;
    angleX += 0.1f;
    angleY += 0.2f;
    if (angleX > 360) angleX -= 360;
    if (angleY > 360) angleY -= 360;
    glutPostRedisplay();
}


// Funções para compilar shaders
unsigned int compileShader(unsigned int type, const char* source) {
    unsigned int id = glCreateShader(type);
    glShaderSource(id, 1, &source, nullptr);
    glCompileShader(id);

    // Verificar erros de compilação
    int success;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(id, 512, nullptr, infoLog);
        std::cerr << "Erro na compilação do shader: " << infoLog << std::endl;
    }
    return id;
}

unsigned int createShaderProgram(const char* vertexShader, const char* fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    // Verificar erros de linkagem
    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cerr << "Erro na linkagem do programa: " << infoLog << std::endl;
    }

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}



void setupBox(unsigned int &VAO, unsigned int &VBO, unsigned int &EBO, unsigned int &catVAO, unsigned int &catVBO, unsigned int &catEBO) {
    // Configurar VAO, VBO e EBO para a caixa de areia e o cubo
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // Carregar vértices
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCubo), verticesCubo, GL_STATIC_DRAW);

    // Carregar índices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesCubo), indicesCubo, GL_STATIC_DRAW);

    // Configurar atributos de posição (x, y, z)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Configurar atributos de cor (r, g, b)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Configurar VAO, VBO e EBO para a cabeça de gato
    glGenVertexArrays(1, &catVAO);
    glGenBuffers(1, &catVBO);
    glGenBuffers(1, &catEBO);

    glBindVertexArray(catVAO);

    // Carregar vértices da cabeça de gato
    glBindBuffer(GL_ARRAY_BUFFER, catVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(catHeadVertices), catHeadVertices, GL_STATIC_DRAW);

    // Carregar índices da cabeça de gato
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, catEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(catHeadIndices), catHeadIndices, GL_STATIC_DRAW);

    // Configurar atributos de posição (x, y, z)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}


// Função principal
/*
int main() {
    // Inicializando o GLFW
    if (!glfwInit()) {
        std::cerr << "Erro ao inicializar o GLFW" << std::endl;
        return -1;
    }

    // Definindo a versão do OpenGL e o perfil Core
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // OpenGL 3.x
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // OpenGL 3.3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Usar Core Profile

    // Criando a janela com GLFW
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Moderno", NULL, NULL);
    if (!window) {
        std::cerr << "Erro ao criar a janela" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);  // Faz a janela atual como o contexto OpenGL

    // **Aqui colocamos a linha para habilitar GLEW experimental**
    glewExperimental = GL_TRUE;  // Habilita extensões modernas no GLEW

    // Inicializando o GLEW após habilitar experimental
    if (glewInit() != GLEW_OK) {
        std::cerr << "Erro ao inicializar o GLEW" << std::endl;
        return -1;
    }

    // Verifica a versão do OpenGL disponível
    std::cout << "Versão do OpenGL: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;

    // Loop de renderização (simples)
    while (!glfwWindowShouldClose(window)) {
        // Limpa a tela
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Swap de buffers e eventos
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Finaliza o GLFW
    glfwTerminate();
    return 0;
}
*/
const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aColor;
    out vec3 ourColor;

    uniform mat4 transform;

    void main() {
        gl_Position = transform * vec4(aPos, 1.0);
        ourColor = aColor;
    }
)";

const char* fragmentShaderSource = R"(
    #version 330 core
    in vec3 ourColor;
    out vec4 FragColor;

    void main() {
        FragColor = vec4(ourColor, 1.0);
    }
)";


unsigned int createShader(const char* vertexShaderSource, const char* fragmentShaderSource) {
    // Compilar o Vertex Shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    
    // Verificar erros de compilação do Vertex Shader
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "Erro ao compilar o Vertex Shader: " << infoLog << std::endl;
    }

    // Compilar o Fragment Shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Verificar erros de compilação do Fragment Shader
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "Erro ao compilar o Fragment Shader: " << infoLog << std::endl;
    }

    // Criar o Shader Program
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Verificar erros de linkagem do Shader Program
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "Erro ao linkar o Shader Program: " << infoLog << std::endl;
    }

    // Deletar os shaders, pois eles já foram vinculados ao programa
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

// Protótipo da função keyboard
void keyboard(unsigned char key, int x, int y);

int main(int argc, char** argv) {
    // Inicializando a GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Cena 3D com Controle de Teclado");

    // Inicializando GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Erro ao inicializar o GLEW" << std::endl;
        return -1;
    }

    // Criar o Shader Program
    unsigned int shaderProgram = createShader(vertexShaderSource, fragmentShaderSource);

    // Funções de inicialização
    init();  // Sua função de inicialização de OpenGL

    // Usar o shader program
    glUseProgram(shaderProgram);  // Ativando os shaders

    // Configurar a matriz de transformação
    glm::mat4 transform = glm::mat4(1.0f);  // Matriz identidade
    transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, -3.0f)); // Translada para trás
    transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotaciona

    // Obter o local do uniform "transform" no shader
    unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

    // Configurar callbacks
    glutDisplayFunc(display);  // Função de exibição
    glutIdleFunc(idle);        // Função de atualização contínua
    glutKeyboardFunc(keyboard); // Função de controle de teclado

    // Loop principal
    glutMainLoop();

    return 0;
}

// Função de controle do teclado
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'w':  // Alterna entre wireframe e sólido
            wireframe = !wireframe;
            break;
        case 'a':  // Rotaciona no eixo Y
            angleY -= 5.0f;
            break;
        case 'd':  // Rotaciona no eixo Y
            angleY += 5.0f;
            break;
        case 's':  // Rotaciona no eixo X
            angleX += 5.0f;
            break;
        case 'z':  // Aumenta escala
            scale += 0.1f;
            break;
        case 'x':  // Diminui escala
            scale -= 0.1f;
            break;
        case 'i':  // Translada para cima
            translateY += 0.1f;
            break;
        case 'k':  // Translada para baixo
            translateY -= 0.1f;
            break;
        case 'j':  // Translada para a esquerda
            translateX -= 0.1f;
            break;
        case 'l':  // Translada para a direita
            translateX += 0.1f;
            break;
        case 27:  // Tecla ESC para sair
            exit(0);
    }
    glutPostRedisplay();
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