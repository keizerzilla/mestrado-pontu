#!/bin/bash

make mcalc

bin=../bin/mcalc
moment=legendre
cloud=../datasets/bosphorus-outlier-densit225-crop80-icp/neutral/bs066_N_N_0.xyz
valgrind --leak-check=full $bin -m $moment -i $cloud -o stdout -c s

