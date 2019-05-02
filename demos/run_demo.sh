#!/bin/bash

clear
gcc demo.c -o demo -lm
./demo
#pcl_ply2pcd color_cloud.ply color_cloud.pcd
#pcl_viewer color_cloud.pcd

