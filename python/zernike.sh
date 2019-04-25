#!/bin/bash

for ord in {1..13}
do
	for rep in {1..13}
	do
		if [ $ord -ge $rep ]; then
			if [ $(($((ord-rep))%2)) -eq 0 ]; then
				gcc -D ZERNIKE_ORDER=$ord -D ZERNIKE_REPETITION=$rep ../src/mcalc.c -Wall -lm -o ../bin/mcalc
				python3 framework.py $ord $rep
				echo "order $ord and repetition $rep ok!"
			fi
		fi
	done
done

