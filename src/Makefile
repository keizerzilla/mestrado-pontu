# configs
CC = gcc
COMPILER_FLAGS = -std=gnu99 -Wall -O3
LINKER_FLAGS = -lm

# alvos de compilacao
all: mcalc cloudz

# programa extrator de momentos invariantes
mcalc: mcalc.c
	$(CC) mcalc.c $(COMPILER_FLAGS) $(LINKER_FLAGS) -o ../bin/mcalc

# utilitario manipulador de nuvens via linha de comando
cloudz: cloudz.c
	$(CC) cloudz.c $(COMPILER_FLAGS) $(LINKER_FLAGS) -o ../bin/cloudz
