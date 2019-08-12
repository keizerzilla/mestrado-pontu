#!/bin/bash

make -C .. clean
make -C .. all -j4
make -j4 $1
clear
../bin/$1

