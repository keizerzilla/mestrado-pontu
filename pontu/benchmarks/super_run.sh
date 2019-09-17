#!/bin/bash

PROFILER="profile"

make -C .. clean
make -C .. all -j4
make -j4 $1

echo ""
echo "------- RUNNING -------"

if [ "$2" == "$PROFILER" ]; then
	valgrind --leak-check=summary ../bin/$1
else
	../bin/$1
fi

echo "-------  DONE!  -------"
echo ""

