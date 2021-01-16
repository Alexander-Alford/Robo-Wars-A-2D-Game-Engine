
SRC_DIR = src
INC_DIR = include
BIN_DIR = .

EXE = $(BIN_DIR)/main
SRC = $(wildcard $(SRC_DIR)/*.c)

CC = gcc 
INCLUDE_PATHS = -IC:\PROG\include\SDL2 -IC:.\$(INC_DIR)
LIBRARY_PATHS = -LC:\PROG\lib\SDL2
COMPILER_FLAGS = 
LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer



all : $(SRC) 
		$(CC) $(SRC) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(EXE)