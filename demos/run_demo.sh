#!/bin/bash

rm folha_*.pcd
rm cloud_*.pcd

face=../dump/bs000_E_HAPPY_0.xyz

clear
gcc demo.c -o demo -lm
./demo $face

pcl_xyz2pcd $face face.pcd
pcl_viewer -fc 0,0,255 -ps 2 face.pcd -fc 255,0,0 -ps 4 seg.pcd

#pcl_xyz2pcd pass1.xyz pass1.pcd
#pcl_xyz2pcd pass2.xyz pass2.pcd


#declare -i count=0
#for filename in folha*.pcd; do
#    count=$((count + 1))
#    mv $filename "cloud_$count.pcd"
#done

#pcl_viewer cloud*.pcd

