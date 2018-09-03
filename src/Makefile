# OBJS especifica os arquivos do projeto que devem ser compilados
OBJS = mcalc.c

# CC especifica o compilador
CC = gcc

# COMPILER_FLAGS guarda as opcoes de compilacao extra
COMPILER_FLAGS = -Wall

# LINKER_FLAGS aponta para as bibliotecas usadas caso necessario
LINKER_FLAGS = -lm

# OBJ_NAME guarda o nome do executavel
OBJ_NAME = ../bin/mcalc

# executa compilacao
all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
