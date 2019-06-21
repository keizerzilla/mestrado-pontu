# configuration variables
CC = gcc
COMPILER_FLAGS = -Wall -Wextra -Winline -fPIC
LINKER_FLAGS = -lm
SRC_DIR = ./src
OBJ_DIR = ./obj
LIB_DIR = ./lib

# source, object and library folders
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))
SHARED_FILES = $(wildcard $(LIB_DIR)/*.so)
STATIC_FILES = $(wildcard $(LIB_DIR)/*.a)

# making the necessary directories
$(shell mkdir -p $(OBJ_DIR))
$(shell mkdir -p $(LIB_DIR))

# targets to create
all: objects shared static

# compiling object files
objects: $(OBJ_FILES)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c $(COMPILER_FLAGS) $(LINKER_FLAGS) -c -o $@ $^

# creating shared libraries
shared: $(OBJ_FILES)
	gcc -shared -o $(LIB_DIR)/libpontu.so $(OBJ_FILES)

# creating static libraries
static: $(OBJ_FILES)
	ar rcs $(LIB_DIR)/libpontu.a $(OBJ_FILES)

# cleaning the built files
clean:
	rm -f $(OBJ_FILES)
	rm -f $(SHARED_FILES)
	rm -f $(STATIC_FILES)

