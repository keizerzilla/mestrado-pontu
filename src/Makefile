# configs
CC = gcc
COMPILER_FLAGS = -Wall
LINKER_FLAGS = -lm
DIR = ../bin/

# cria dir se nao existir
$(shell mkdir -p $(DIR))

# alvos de compilacao
all: mcalc cloudz nosex normx nosecut

# programa extrator de momentos invariantes
mcalc: mcalc.c
	$(CC) mcalc.c $(COMPILER_FLAGS) $(LINKER_FLAGS) $(CFLAGS) -o $(DIR)mcalc

# utilitario manipulador de nuvens via linha de comando
cloudz: cloudz.c
	$(CC) cloudz.c $(COMPILER_FLAGS) $(LINKER_FLAGS) $(CFLAGS) -o $(DIR)cloudz

# programa extrator de ponta do nariz
nosex: nosex.c
	$(CC) nosex.c $(COMPILER_FLAGS) $(LINKER_FLAGS) $(CFLAGS) -o $(DIR)nosex

# programa extrator de normal a nuvem
normx: normx.c
	$(CC) normx.c $(COMPILER_FLAGS) $(LINKER_FLAGS) $(CFLAGS) -o $(DIR)normx

# programa extrator de região do nariz
nosecut: nosecut.c
	$(CC) nosecut.c $(COMPILER_FLAGS) $(LINKER_FLAGS) $(CFLAGS) -o $(DIR)nosecut

