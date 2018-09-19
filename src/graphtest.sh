#!/bin/bash

graph=" ../bin/cloudgraph "
viewer=" pcl_viewer "
xyz2pcd=" pcl_xyz2pcd "

cloud_xyz=" ../datasets/dump/bs000_N_N_0.xyz "
graph_xyz=" ../datasets/dump/graph_bs000_N_N_0.xyz "

cloud_pcd=" ../datasets/dump/bs000_N_N_0.pcd "
graph_pcd=" ../datasets/dump/graph_bs000_N_N_0.pcd "

param1=" -ps 1 -fc 255,255,255 "
param2=" -ps 1 -fc 255,0,255 "
param3=" -ax 60 "

make
$graph$cloud_xyz$graph_xyz
$xyz2pcd$graph_xyz$graph_pcd
$xyz2pcd$cloud_xyz$cloud_pcd
$viewer$cloud_pcd$param1$graph_pcd$param2$param3
#$viewer$graph_pcd
