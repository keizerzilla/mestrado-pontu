import os
import numpy as np
import pandas as pd
import numpy.linalg as linalg
from open3d import *
from sklearn.decomposition import PCA

LEAF_SIZE = 1.00
OUTLIER_NN = 20
OUTLIER_STD = 2.25
CUT_SIZE = 80.00

def py_ang(v1, v2):
	cosang = np.dot(v1, v2)
	sinang = linalg.norm(np.cross(v1, v2))
	return np.degrees(np.arctan2(sinang, cosang))

def py_unit(v):
	return v / (v**2).sum()**0.5

def preprocessing(input_cloud, cloud, outdir):
	# carrega nuvem
	pcd = read_point_cloud(input_cloud)
	
	# downsample
	pcd = voxel_down_sample(pcd, voxel_size=LEAF_SIZE)
	
	# outlier removal
	pcd, _ = statistical_outlier_removal(pcd,
	                                     nb_neighbors=OUTLIER_NN,
	                                     std_ratio=OUTLIER_STD)
	
	# translada para centro
	pcd = np.asarray(pcd.points)
	centroid = np.mean(pcd, axis=0)
	origin = np.array([0, 0, 0])
	direction = origin - centroid
	pcd = pcd + direction
	
	# alinhamento
	pca = PCA(n_components=3)
	pca.fit(pcd)
	v = pca.components_
	
	x = np.array([1, 0, 0])
	y = np.array([0, 1, 0])
	z = v[2]
	
	x = x * np.dot(z, x)
	y = np.cross(z, x)
	
	x = py_unit(x)
	y = py_unit(y)
	z = py_unit(z)
	
	rot = np.absolute(np.array([x, y, z]))
	pcd = np.matmul(pcd, rot)
	
	# corte
	idx = linalg.norm(pcd, axis=1) <= CUT_SIZE
	pcd = pcd[idx]
	
	# save
	df = pd.DataFrame(pcd)
	df.to_csv(outdir + cloud, header=None, index=None, sep=' ')
	print(cloud, "OK!")

if __name__ == "__main__":
	directory = "../datasets/bosphorus/neutral/"
	outdir = "../datasets/bosphorus-tutu/neutral/"
	
	for cloud in os.listdir(directory):
		if ".xyz" in cloud:
			input_cloud = directory + cloud
			preprocessing(input_cloud, cloud, outdir)
			
			
