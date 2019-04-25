#!/bin/bash

clear
gcc -D ZERNIKE_ORDER=2 -D ZERNIKE_REPETITION=6 demo.c -o demo -lm
./demo

