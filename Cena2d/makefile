# Definir o compilador
CC = g++

# Definir flags de compilação
CFLAGS = -g -Wall

# Nome do arquivo executável
TARGET = cena2d

# Arquivos de código fonte
SRCS = cena2d.cpp

# Bibliotecas necessárias (adicionei GLU e GLUT aqui)
LIBS = -lglfw -lGL -lGLEW -lGLU -lglut

# Regras do Makefile

# Regra padrão: compilar o projeto
all: $(TARGET)

# Regra para criar o executável
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET) $(LIBS)

# Limpar arquivos compilados
clean:
	rm -f $(TARGET)
