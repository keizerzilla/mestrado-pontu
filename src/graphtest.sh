#!/bin/bash

graph=" ../bin/cloudgraph "
viewer=" pcl_viewer "
xyz2pcd=" pcl_xyz2pcd "

cloud_xyz=" ../datasets/bosphorus_no-outliers/Neutrals/bs000_N_N_0.xyz "
lendmark_xyz=" ../datasets/lendmarks/lendmark_bs000_N_N_0.xyz "

cloud_pcd=" ../datasets/bosphorus_pcd/Neutrals/bs000_N_N_0.pcd "
lendmark_pcd=" ../datasets/lendmarks/lendmark_bs000_N_N_0.pcd "

param1=" -ps 1 -fc 0,0,255 "
param2=" -ps 4 -fc 255,0,0 "
param3=" -position 0 0 -2 "

make
$graph$cloud_xyz$lendmark_xyz
$xyz2pcd$lendmark_xyz$lendmark_pcd
$viewer$cloud_pcd$param1$lendmark_pcd$param2
