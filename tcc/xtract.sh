#!/bin/bash

clear
make -C ../src/

neutral_path='neutral/'
hard_path='hard/'

rm -rf $neutral_path*
rm -rf $hard_path*

extractor=$1
cut=$2

./extraction.py $extractor $cut ../data/set/bs_outliers_free/Neutrals/ $neutral_path
./extraction.py $extractor $cut ../data/set/bs_outliers_free/Neutrals/ $hard_path 1
./classif.py $extractor $cut $hard_path$extractor.ft $hard_path$extractor.cl $neutral_path$extractor.ft $neutral_path$extractor.cl

