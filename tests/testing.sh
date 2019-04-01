#!/bin/bash

clear
make
./testing
pcl_viewer -multiview 1 mask_0.pcd mask_1.pcd mask_2.pcd mask_3.pcd

