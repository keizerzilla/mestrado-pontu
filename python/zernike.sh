#!/bin/bash

for x in {0..3}
do
	for y in {0..3}
	do
		for z in {0..3}
		do
			gcc -D SPHERIC_ORDER_X=$x -D SPHERIC_ORDER_Y=$y -D SPHERIC_ORDER_Z=$z ../src/mcalc.c -Wall -lm -o ../bin/mcalc
			python3 framework.py "spheric" "$x-$y-$z"
			echo "$x-$y-$z ok!"
		done
	done
done

