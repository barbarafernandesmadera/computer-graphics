# Nome do executável
TARGET = ./bin/cena3d

# Compilador e flags
CXX = g++
CXXFLAGS = -Wall -g -std=c++11

# Diretórios
INCLUDES = -I./includes
OBJ_DIR = ./obj
BIN_DIR = ./bin

# Bibliotecas que precisam ser linkadas (incluindo GLUT e GLU)
LIBS = -lGL -lGLEW -lglfw -lglut -lGLU

# Fonte
SRC = ./cena3d.cpp

# Objeto
OBJ = $(OBJ_DIR)/cena3d.o

# Objetivo padrão (default)
all: create_dirs $(TARGET)

# Regra para criar os diretórios 'bin' e 'obj', se não existirem
create_dirs:
	mkdir -p $(OBJ_DIR)
	mkdir -p $(BIN_DIR)

# Regra para compilar o executável
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ) $(LIBS)

# Regra para compilar o arquivo fonte
$(OBJ): $(SRC)
	$(CXX) $(CXXFLAGS) -c $(SRC) -o $(OBJ)

# Limpeza dos arquivos objetos e executável
clean:
	rm -f $(OBJ_DIR)/*.o $(TARGET)

# Regra para rodar o programa
run: all
	./$(TARGET)
