#!/bin/bash

# HU
hft='hu.ft'
hcl='hu.cl'

# ZERNIKE
zft='zernike.ft'
zcl='zernike.cl'

# LEGENDRE
lft='legendre.ft'
lcl='legendre.cl'

# TCHEBYCHEV
tft='tchebychev.ft'
tcl='tchebychev.cl'

# PATHS
in_neutral='../data/set/bs_outliers_free/Neutrals/'
in_nonneutral='../data/set/bs_outliers_free/NonNeutrals/'

out_neutral='../data/results/Neutrals/'
out_nonneutral='../data/results/NonNeutrals/'
out_hard='../data/results/HardMode/'
out_full='../data/results/Full/'

# PREP
clear

printf "%-*s%-*s%-*s%-*s\n" "16" "EXTRATOR" "16" "CORTE" "16" "CLASSIFICADOR" "16" "SCORE"

# CLASSIFICACAO
for cutsize in 50 60 70 80 90 -1; do
	./classif.py hu $cutsize $out_hard$cutsize/$hft $out_hard$cutsize/$hcl $out_neutral$cutsize/$hft $out_neutral$cutsize/$hcl
	./classif.py zernike $cutsize $out_hard$cutsize/$zft $out_hard$cutsize/$zcl $out_neutral$cutsize/$zft $out_neutral$cutsize/$zcl
	./classif.py legendre $cutsize $out_hard$cutsize/$lft $out_hard$cutsize/$lcl $out_neutral$cutsize/$lft $out_neutral$cutsize/$lcl
	./classif.py tchebychev $cutsize $out_hard$cutsize/$tft $out_hard$cutsize/$tcl $out_neutral$cutsize/$tft $out_neutral$cutsize/$tcl
done

for cutsize in 50 60 70 80 90 -1; do
	./classif.py hu $cutsize $out_hard$cutsize/$hft $out_hard$cutsize/$hcl $out_nonneutral$cutsize/$hft $out_nonneutral$cutsize/$hcl
	./classif.py zernike $cutsize $out_hard$cutsize/$zft $out_hard$cutsize/$zcl $out_nonneutral$cutsize/$zft $out_nonneutral$cutsize/$zcl
    ./classif.py legendre $cutsize $out_hard$cutsize/$lft $out_hard$cutsize/$lcl $out_nonneutral$cutsize/$lft $out_nonneutral$cutsize/$lcl
	./classif.py tchebychev $cutsize $out_hard$cutsize/$tft $out_hard$cutsize/$tcl $out_nonneutral$cutsize/$tft $out_nonneutral$cutsize/$tcl
done
