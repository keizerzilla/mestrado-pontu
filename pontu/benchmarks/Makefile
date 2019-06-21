# configuration variables
CC = gcc
COMPILER_FLAGS = -Wall -Werror -fPIC
LINKER_FLAGS = ../lib/libpontu.a -lm
BIN_DIR = ../../bin

# making the necessary directories
$(shell mkdir -p $(BIN_DIR))

# source, object and library folders
SRC_FILES = $(wildcard *.c)
EXE_FILES = $(patsubst %.c,%,$(SRC_FILES))

# targets to create
all: $(EXE_FILES)

# compilation
$(EXE_FILES): %: %.c
	$(CC) $(COMPILER_FLAGS) -o $(BIN_DIR)/$@ $< $(LINKER_FLAGS)

