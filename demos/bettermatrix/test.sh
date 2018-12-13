#!/bin/bash

make
valgrind --leak-check=full ../../bin/bettermatrix ../../datasets/bosphorus-tcc/neutral/bs071_N_N_0.xyz

