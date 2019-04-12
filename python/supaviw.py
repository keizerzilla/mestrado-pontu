import os
import subprocess
import numpy as np
import pandas as pd
import numpy.linalg as linalg
import copy
from open3d import *
from sklearn.decomposition import PCA

def supaviw(input_cloud, icp_cloud, cloud):
	pcd = read_point_cloud(input_cloud)
	reg = read_point_cloud(icp_cloud)
	
	pcd.paint_uniform_color([0, 1, 1])
	reg.paint_uniform_color([1, 1, 0])
	
	print(cloud)
	draw_geometries([pcd, reg])


folder = "../datasets/tutu/neutral/"
icp = "../datasets/bosphorus-outlier-densit200-crop80-icp/neutral/"
for cloud in os.listdir(folder):
	if ".xyz" in cloud:
		input_cloud = folder + cloud
		icp_cloud = icp + cloud
		supaviw(input_cloud, icp_cloud, cloud)


