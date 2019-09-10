#!/bin/bash

make -C .. clean
make -C .. all -j4
make -j4 $1
#clear
echo ""
echo "------- RUNNING -------"
#valgrind --leak-check=summary ../bin/$1
../bin/$1
echo "-------  DONE!  -------"
echo ""

