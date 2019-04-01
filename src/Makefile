# configs
CC = gcc
COMPILER_FLAGS = -Wall
LINKER_FLAGS = -lm

# alvos de compilacao
all: mcalc cloudz

# programa extrator de momentos invariantes
mcalc: mcalc.c
	$(CC) mcalc.c $(COMPILER_FLAGS) $(LINKER_FLAGS) $(CFLAGS) -o ../bin/mcalc

# utilitario manipulador de nuvens via linha de comando
cloudz: cloudz.c
	$(CC) cloudz.c $(COMPILER_FLAGS) $(LINKER_FLAGS) $(CFLAGS) -o ../bin/cloudz

