# targets de compilacao
all: mcalc cloudz radialcut

# programa extrator de momentos invariantes
mcalc: mcalc.c
	gcc mcalc.c -lm -Wall -o ../bin/mcalc

# utilitario manipulador de nuvens via linha de comando
cloudz: cloudz.c
	gcc cloudz.c -lm -Wall -o ../bin/cloudz

# utilitario que corta a nuvem de formas radiais
radialcut: radialcut.c
	gcc radialcut.c -lm -Wall -o ../bin/radialcut
