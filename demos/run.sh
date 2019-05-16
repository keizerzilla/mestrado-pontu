#!/bin/bash

clear
gcc demo.c -o demo -lm
./demo

#pcl_xyz2pcd ../dump/bunny_raw.xyz ../dump/bunny_raw.pcd
#pcl_xyz2pcd ../dump/bunny_rec.xyz ../dump/bunny_rec.pcd
#pcl_viewer -multiview 1 ../dump/bunny_raw.pcd ../dump/bunny_rec.pcd

