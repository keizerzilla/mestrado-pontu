#!/bin/bash

clear
gcc demo.c -o demo -lm
./demo ../dump/bs000_N_N_0.xyz
pcl_xyz2pcd ../dump/nose.xyz ../dump/nose.pcd
pcl_viewer -ps 3 ../dump/nose.pcd

