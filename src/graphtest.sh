#!/bin/bash

prog=" ../bin/kdtest "
viewer=" pcl_viewer "
xyz2pcd=" pcl_xyz2pcd "

cloud_xyz=" ../datasets/dump/cloud.xyz " # input
graph_xyz=" ../datasets/dump/graph_cloud.xyz "
nose_xyz=" ../datasets/dump/nose_cloud.xyz "

cloud_pcd=" ../datasets/dump/cloud.pcd "
graph_pcd=" ../datasets/dump/graph_cloud.pcd "
nose_pcd=" ../datasets/dump/nose_cloud.pcd "

param1=" -ps 1 -fc 0,0,255 "
param2=" -ps 5 -fc 255,0,0 "
param3=" -ax 60 -ax_pos 100,25,0 "

make kdtest

$prog$cloud_xyz$graph_xyz$nose_xyz
$xyz2pcd$graph_xyz$graph_pcd
$xyz2pcd$cloud_xyz$cloud_pcd
$xyz2pcd$nose_xyz$nose_pcd
$viewer$cloud_pcd$param1$graph_pcd$param2$param3
#$viewer$graph_pcd$param2$param3
#$viewer$cloud_pcd$param1$nose_pcd$param2$param3
