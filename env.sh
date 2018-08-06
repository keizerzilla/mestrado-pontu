#!/bin/bash

if [ ! -d "bin/" ]; then
	mkdir bin/
fi

if [ ! -d "datasets/" ]; then
	mkdir datasets/
fi

if [ ! -d "dump/" ]; then
	mkdir dump/
fi

echo "Done!"
