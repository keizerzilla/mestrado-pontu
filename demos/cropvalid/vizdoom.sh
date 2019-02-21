#!/bin/bash

make
./cropvalid

pcl_xyz2pcd par1_fh.xyz par1_fh.pcd
pcl_xyz2pcd par1_sh.xyz par1_sh.pcd
pcl_xyz2pcd par2_fh.xyz par2_fh.pcd
pcl_xyz2pcd par2_sh.xyz par2_sh.pcd

pcl_xyz2pcd sub1.xyz sub1.pcd
pcl_xyz2pcd sub2.xyz sub2.pcd

pcl_xyz2pcd upper.xyz upper.pcd
pcl_xyz2pcd lower.xyz lower.pcd

pcl_viewer -ps 2 -fc 255,0,0 par1_fh.pcd -ps 2 -fc 0,255,0 par1_sh.pcd -ps 2 -fc 0,0,255 par2_fh.pcd -ps 2 -fc 255,0,255 par2_sh.pcd
#pcl_viewer -ps 2 -fc 255,0,0 sub1.pcd -ps 2 -fc 0,255,0 sub2.pcd
#pcl_viewer -ps 2 -fc 255,0,0 upper.pcd -ps 2 -fc 0,255,0 upper.pcd
#pcl_viewer -ps 2 -fc 255,0,0 lower.pcd -ps 2 -fc 0,255,0 lower.pcd



