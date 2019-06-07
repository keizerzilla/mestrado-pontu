#!/bin/bash

make -C ../pontu clean
make -C ../pontu
gcc benchmark.c -L../pontu/lib/static -lpontu_core -lpontu_features \
    -lpontu_sampling -lm -o benchmark
clear
./benchmark

