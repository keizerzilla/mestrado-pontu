import os
import subprocess
import numpy as np
import pandas as pd
import numpy.linalg as linalg
from open3d import *

def unit_vector(v):
	return v / (v**2).sum()**0.5

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
	prog = "../bin/normx"
	cloud = "../dump/temp.xyz"
	cmd = [prog, cloud]
	
	if isinstance(pcd, np.ndarray):
		df = pd.DataFrame(pcd)
	else:
		df = pd.DataFrame(np.asarray(pcd.points))
	
	df.to_csv(cloud, header=None, index=None, sep=' ')
	
	ans = subprocess.run(cmd, stdout=subprocess.PIPE).stdout
	ans = ans[:-2].decode("utf-8")
	
	normal = np.array([float(x) for x in ans.split(' ')])
	
	return normal

def load_cloud(filepath):
	return read_point_cloud(filepath)

def downsample(pcd, leafsize=2.00):
	return voxel_down_sample(pcd, leafsize)

def outlier_removal(pcd, nn=50, r=10):
	out, _ = radius_outlier_removal(pcd, nn, r)
	
	return out

def segmentation(pcd, cut=80):
	nosetip = get_nosetip(pcd)
	
	if isinstance(pcd, np.ndarray):
		data = pcd
	else:
		data = np.asarray(pcd.points)
	
	origin = np.array([0, 0, 0])
	direction = origin - nosetip
	data = data + direction
	
	indexes = linalg.norm(data, axis=1) <= cut
	data = data[indexes]
	
	return data

def alignment(data, by_nose=False):
	x = np.array([1, 0, 0])
	y = np.array([0, 1, 0])
	z = get_normal(data)
	
	y = y * np.dot(z, y)
	x = np.cross(z, y)
	
	x = unit_vector(x)
	y = unit_vector(y)
	z = unit_vector(z)
	
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
	pcd = downsample(pcd, leafsize)
	pcd = outlier_removal(pcd, nn, r)
	#pcd = segmentation(pcd, cut)
	#pcd = alignment(pcd)
	save_result(pcd, outdir + cloud)

def batch_preprocessing(folder, outdir, leafsize=2.00, nn=50, r=10, cut=80):
	os.makedirs(outdir, exist_ok=True)
	for cloud in os.listdir(folder):
		if ".xyz" in cloud:
			input_cloud = folder + cloud
			preprocessing(input_cloud, cloud, outdir, leafsize, nn, r, cut)
			print(cloud, "ok!")

if __name__ == "__main__":
	folder = ["../datasets/bosphorus/neutral/",
	          "../datasets/bosphorus/nonneutral/"]
	outdir = ["../datasets/bosphorus-tutu/neutral/",
	          "../datasets/bosphorus-tutu/nonneutral/"]
	
	for f, o in zip(folder, outdir):
		batch_preprocessing(f, o)
	
