all: char-ext pcview centerprofile

char-ext: char-ext.c
	gcc char-ext.c -o ../bin/char-ext -lm
	
pcview: cloudview.c
	gcc cloudview.c -o ../bin/cloudview -lm -lGL -lGLU -lglut
	
cloudz: centerprofile.c
	gcc centerprofile.c -o ../bin/centerprofile -lm
	
