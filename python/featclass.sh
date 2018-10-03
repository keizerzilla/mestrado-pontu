#!/bin/bash

make mcalc -C ../src/
python3 features.py
python3 classification.py
