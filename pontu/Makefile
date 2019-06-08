# configuration variables
CC = gcc
COMPILER_FLAGS = -Wall -Werror -fpic
SHARED_FLAGS = -shared
LINKER_FLAGS = -lm
SRC_DIR = src
OBJ_DIR = obj
LIB_DIR = lib

# shared and static folders
SHARED_DIR = $(LIB_DIR)/shared
STATIC_DIR = $(LIB_DIR)/static

# source, object and library folders
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))
SHARED_FILES = $(wildcard $(SHARED_DIR)/*.so)
STATIC_FILES = $(wildcard $(SHARED_DIR)/*.a)

# library files
LIB_CORE = $(OBJ_DIR)/calc.o $(OBJ_DIR)/vector3.o $(OBJ_DIR)/plane.o \
	       $(OBJ_DIR)/cloud.o $(OBJ_DIR)/kdtree.o
LIB_FEATURES = $(OBJ_DIR)/matrix.o $(OBJ_DIR)/extraction.o \
               $(OBJ_DIR)/chebyshev.o $(OBJ_DIR)/hu.o $(OBJ_DIR)/legendre.o
LIB_SAMPLING = $(OBJ_DIR)/voxelgrid.o

# cria dir se nao existir
$(shell mkdir -p $(OBJ_DIR))
$(shell mkdir -p $(LIB_DIR))
$(shell mkdir -p $(SHARED_DIR))
$(shell mkdir -p $(STATIC_DIR))

# targets to create
all: objects shared static

# compiling object files
objects: $(OBJ_FILES)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c $(COMPILER_FLAGS) $(LINKER_FLAGS) -c -o $@ $^

# creating shared libraries
shared: $(OBJ_FILES)
	gcc -shared -o $(SHARED_DIR)/libpontu_core.so $(LIB_CORE)
	gcc -shared -o $(SHARED_DIR)/libpontu_features.so $(LIB_FEATURES)
	gcc -shared -o $(SHARED_DIR)/libpontu_sampling.so $(LIB_SAMPLING)

# creating static libraries
static: $(OBJ_FILES)
	ar rcs $(STATIC_DIR)/libpontu_core.a $(LIB_CORE)
	ar rcs $(STATIC_DIR)/libpontu_features.a $(LIB_FEATURES)
	ar rcs $(STATIC_DIR)/libpontu_sampling.a $(LIB_SAMPLING)

# cleaning the built files
clean:
	rm -f $(OBJ_FILES)
	rm -f $(SHARED_FILES)
	rm -f $(STATIC_FILES)

