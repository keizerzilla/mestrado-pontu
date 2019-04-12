import os
import subprocess
import numpy as np
import pandas as pd
import numpy.linalg as linalg
import copy
from open3d import *
from sklearn.decomposition import PCA

def get_nosetip_normal(cloud):
	cmd = ["../bin/nosext", cloud]
	ans = subprocess.run(cmd, stdout=subprocess.PIPE).stdout
	l = ans[:-2].decode("utf-8").split('\n')
	normal = np.array([float(x) for x in l[0].split(' ')])
	nosetip = np.array([float(x) for x in l[1].split(' ')])
	
	return nosetip, normal

def draw_registration_result(source, target, transformation):
	source_temp = copy.deepcopy(source)
	target_temp = copy.deepcopy(target)
	source_temp.paint_uniform_color([1, 0.706, 0])
	target_temp.paint_uniform_color([0, 0.651, 0.929])
	source_temp.transform(transformation)
	draw_geometries([source_temp, target_temp])

def icp(source_cloud, target_cloud, threshold=0.0002):
	source = read_point_cloud(source_cloud)
	target = read_point_cloud(target_cloud)
	
	nose_source, normal_source = get_nosetip_normal(source_cloud)
	nose_target, normal_target = get_nosetip_normal(target_cloud)
	
	direction = nose_target - nose_source
	
	trans_tutu = np.asarray([[1.0, 0.0, 0.0, direction[0]],
                             [0.0, 1.0, 0.0, direction[1]],
                             [0.0, 0.0, 1.0, direction[2]],
	                         [0.0, 0.0, 0.0, 1.0]])
	
	reg_p2p = registration_icp(source, target, threshold,
                               TransformationEstimationPointToPoint())
	

folder = "../datasets/tutu/neutral/"
clouds = [folder + x for x in os.listdir(folder) if ".xyz" in x]

for i in range(len(clouds)-1):
	icp(clouds[i], clouds[i+1])

