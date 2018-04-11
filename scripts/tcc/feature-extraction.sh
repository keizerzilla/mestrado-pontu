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
out_super='../data/results/SuperHard/'

# PREP
clear
make -C ../src/

# EXTRACAO
for cutsize in 50 60 70 80 90 -1; do
	# EXTRACAO NEUTRAL
	rm -rf $out_neutral$cutsize/*
	./extraction.py hu $cutsize $in_neutral $out_neutral$cutsize/
	./extraction.py zernike $cutsize $in_neutral $out_neutral$cutsize/
	./extraction.py legendre $cutsize $in_neutral $out_neutral$cutsize/
	./extraction.py tchebychev $cutsize $in_neutral $out_neutral$cutsize/

	# EXTRACAO NONEUTRAL
	rm -rf $out_nonneutral$cutsize/*
	./extraction.py hu $cutsize $in_nonneutral $out_nonneutral$cutsize/
	./extraction.py zernike $cutsize $in_nonneutral $out_nonneutral$cutsize/
	./extraction.py legendre $cutsize $in_nonneutral $out_nonneutral$cutsize/
	./extraction.py tchebychev $cutsize $in_nonneutral $out_nonneutral$cutsize/

	# EXTRACAO HARDMODE
	rm -rf $out_hard$cutsize/*
	./extraction.py hu $cutsize $in_neutral $out_hard$cutsize/ 1
	./extraction.py zernike $cutsize $in_neutral $out_hard$cutsize/ 1
	./extraction.py legendre $cutsize $in_neutral $out_hard$cutsize/ 1
	./extraction.py tchebychev $cutsize $in_neutral $out_hard$cutsize/ 1
	
	# EXTRACAO SUPERHARD
	rm -rf $out_super$cutsize/*
	./extraction.py hu $cutsize $in_nonneutral $out_super$cutsize/ 1
	./extraction.py zernike $cutsize $in_nonneutral $out_super$cutsize/ 1
	./extraction.py legendre $cutsize $in_nonneutral $out_super$cutsize/ 1
	./extraction.py tchebychev $cutsize $in_nonneutral $out_super$cutsize/ 1
done
