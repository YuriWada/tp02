# Compilador e flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c17 -g

# Diretórios
SRC = src
OBJ = obj
BIN = bin
INCLUDE = include

# Arquivos
SOURCES = $(wildcard $(SRC)/*.c)
OBJECTS = $(patsubst $(SRC)/%.c,$(OBJ)/%.o,$(SOURCES))
EXEC = $(BIN)/tp3.out

# Regras principais
all: $(EXEC)

$(EXEC): $(OBJECTS)
	mkdir -p $(BIN)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ)/%.o: $(SRC)/%.c
	mkdir -p $(OBJ)
	$(CC) $(CFLAGS) -I$(INCLUDE) -c $< -o $@

clean:
	rm -rf $(OBJ)/*.o $(BIN)/*

# Phony targets
.PHONY: all clean
