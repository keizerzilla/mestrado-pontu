#!/bin/bash

make cloudz
../bin/cloudz ../dump/bunny.xyz rotatez 90 ../dump/bunnyrot.xyz
pcl_xyz2pcd ../dump/bunnyrot.xyz ../dump/bunnyrot.pcd
pcl_xyz2pcd ../dump/bunny.xyz ../dump/bunny.pcd
pcl_viewer -ax 0.2 -multiview 1 ../dump/bunny.pcd ../dump/bunnyrot.pcd
