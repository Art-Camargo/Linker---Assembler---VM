# Nome do executável
TARGET = main

# Compilador
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -g

# Lista todos os arquivos .cpp do diretório atual
SRCS = $(wildcard *.cpp)

# Converte os nomes dos .cpp em .o
OBJS = $(SRCS:.cpp=.o)

# Regra principal
all: $(TARGET)

# Como gerar o executável
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Regra para compilar cada .cpp em .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpa só os arquivos .o
clean:
	rm -f $(OBJS)

.PHONY: all clean
