#!/bin/bash

gcc -c -Wall -Wextra -Winline -Werror -Wuninitialized -fPIC src/$1.c -lm -o $1.o 2> $1_output.txt
more $1_output.txt

rm $1.o
rm $1_output.txt

