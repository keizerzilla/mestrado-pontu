#!/bin/bash

#clear
gcc demo.c -o demo -lm
./demo ../dump/bs000_N_N_0.xyz
#pcl_xyz2pcd landmarks.xyz landmarks.pcd
#pcl_viewer -fc 0,0,255 -ps 2 ../dump/bs000_N_N_0.pcd -fc 255,0,0 -ps 5 landmarks.pcd

