#!/bin/bash

arg1=../../datasets/bosphorus-outlier-densit200-crop80-icp/nonneutral/bs000_E_HAPPY_0.xyz
arg2=../../dump/slice.xyz

make
../../bin/alydist $arg1 $arg2
