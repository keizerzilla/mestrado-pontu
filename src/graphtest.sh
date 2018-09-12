#!/bin/bash

graph=" ../bin/cloudgraph "
viewer=" pcl_viewer "
xyz2pcd=" pcl_xyz2pcd "

cloud_xyz=" ../datasets/bosphorus_no-outliers/Neutrals/bs000_N_N_0.xyz "
graph_xyz=" ../datasets/dump/bs000_N_N_0.xyz "

cloud_pcd=" ../datasets/bosphorus_no-outliers/Neutrals/bs000_N_N_0.pcd "
graph_pcd=" ../datasets/dump/bs000_N_N_0.pcd "

param1=" -ps 1 -fc 0,0,255 "
param2=" -ps 4 -fc 255,0,0 "
param3=" -position 0 0 -2 "

make
#valgrind --leak-check=full $graph$cloud_xyz$graph_xyz
$graph$cloud_xyz$graph_xyz
$xyz2pcd$graph_xyz$graph_pcd
$viewer$cloud_pcd$param1$graph_pcd$param2
#$viewer$graph_pcd
