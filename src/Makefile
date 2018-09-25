# targets de compilacao
all: mcalc cloudz cloudgraph

# programa extrator de momentos invariantes
mcalc: mcalc.c
	gcc mcalc.c -lm -Wall -o ../bin/mcalc

# utilitario manipulador de nuvens via linha de comando
cloudz: cloudz.c
	gcc cloudz.c -lm -Wall -o ../bin/cloudz

# cria grafo a partir de nuvem
cloudgraph: cloudgraph.c
	gcc cloudgraph.c -lm -Wall -o ../bin/cloudgraph

