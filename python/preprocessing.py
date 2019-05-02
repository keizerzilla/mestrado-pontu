import os
import math
import subprocess
import numpy as np
import pandas as pd
import numpy.linalg as linalg
from open3d import *
from sklearn.decomposition import PCA

def unit_vector(v):
	return v / (v**2).sum()**0.5

def angle(v1, v2):
	angle = np.arccos(np.dot(v1, v2)/(np.linalg.norm(v1) * np.linalg.norm(v2)))
	return angle

def get_nosetip(pcd):
	prog = "../bin/nosex"
	cloud = "../dump/temp.xyz"
	cmd = [prog, cloud]
	
	if isinstance(pcd, np.ndarray):
		df = pd.DataFrame(pcd)
	else:
		df = pd.DataFrame(np.asarray(pcd.points))
	
	df.to_csv(cloud, header=None, index=None, sep=' ')
	
	ans = subprocess.run(cmd, stdout=subprocess.PIPE).stdout
	ans = ans[:-2].decode("utf-8")
	
	nosetip = np.array([float(x) for x in ans.split(' ')])
	
	return nosetip

def get_normal(pcd):
	if isinstance(pcd, np.ndarray):
		df = pd.DataFrame(pcd)
	else:
		df = pd.DataFrame(np.asarray(pcd.points))
	
	prog = "../bin/normx"
	cloud = "../dump/temp.xyz"
	cmd = [prog, cloud]
	
	df.to_csv(cloud, header=None, index=None, sep=' ')
	
	ans = subprocess.run(cmd, stdout=subprocess.PIPE).stdout
	ans = ans[:-2].decode("utf-8")
	
	normal = np.array([float(x) for x in ans.split(' ')])
	
	return normal

def load_cloud(filepath):
	return read_point_cloud(filepath)

def downsample(pcd, leafsize=2.00):
	if isinstance(pcd, np.ndarray):
		data = PointCloud()
		data.points = Vector3dVector(pcd)
	else:
		data = pcd
	
	return voxel_down_sample(data, leafsize)

def outlier_removal(pcd, nn=50, r=10):
	out, _ = radius_outlier_removal(pcd, nn, r)
	
	return out

def translation(pcd):
	if isinstance(pcd, np.ndarray):
		data = pcd
	else:
		data = np.asarray(pcd.points)
		
	#nosetip = np.mean(data, axis=0)
	nosetip = get_nosetip(data)
	origin = np.array([0, 0, 0])
	direction = origin - nosetip
	data = data + direction
	
	return data

def segmentation(pcd, cut=80):
	if isinstance(pcd, np.ndarray):
		data = pcd
	else:
		data = np.asarray(pcd.points)
	
	indexes = linalg.norm(data, axis=1) <= cut
	data = data[indexes]
	
	return data

def alignment(data):
	pca = PCA(n_components=3)
	pca.fit(data)
	
	x = pca.components_[1]
	y = pca.components_[0]
	z = pca.components_[2]
	
	rotation_matrix = np.absolute(np.array([x, y, z]))
	aligned = np.matmul(data, rotation_matrix)
	
	return aligned

def save_result(data, outfile):
	if not isinstance(data, np.ndarray):
		data = np.asarray(data.points)
	
	df = pd.DataFrame(data)
	df.to_csv(outfile, header=None, index=None, sep=' ')

def preprocessing(filepath, cloud, outdir, leafsize=2.00, nn=50, r=10, cut=80):
	pcd = load_cloud(filepath)
	#pcd = downsample(pcd, leafsize)
	pcd = outlier_removal(pcd, nn, r)
	#pcd = translation(pcd)
	#pcd = segmentation(pcd, cut)
	#pcd = alignment(pcd)
	save_result(pcd, outdir + cloud)

def batch_preprocessing(folder, outdir, leafsize=2.00, nn=50, r=10, cut=80):
	os.makedirs(outdir, exist_ok=True)
	for cloud in os.listdir(folder):
		if ".xyz" in cloud:
			input_cloud = folder + cloud
			preprocessing(input_cloud, cloud, outdir, leafsize, nn, r, cut)
			print(cloud, "preprocessed Ok!")

if __name__ == "__main__":
	folder = "../datasets/bosphorus-nose/neutral/"
	outdir = "../datasets/bosphorus-nose-preprocessed/neutral/"
	
	batch_preprocessing(folder, outdir)
	
	
	
	
	
	
	
	
