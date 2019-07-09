#!/bin/bash

make -C .. all -j4
make -j4 ritter
clear
../bin/ritter
pcl_viewer blured_bunny.pcd

