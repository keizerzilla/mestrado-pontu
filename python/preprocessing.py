import os
import subprocess
import numpy as np
import pandas as pd
import numpy.linalg as linalg
import copy
from scipy.spatial.distance import cdist
from open3d import *
from sklearn.decomposition import PCA

def get_nosetip_normal(cloud):
	cmd = ["../bin/nosext", cloud]
	ans = subprocess.run(cmd, stdout=subprocess.PIPE).stdout
	l = ans[:-2].decode("utf-8").split('\n')
	normal = np.array([float(x) for x in l[0].split(' ')])
	nosetip = np.array([float(x) for x in l[1].split(' ')])
	
	return nosetip, normal

def unit_vector(v):
	return v / (v**2).sum()**0.5

def gram_schmidt_columns(X):
	Q, R = np.linalg.qr(X)
	return Q

def preprocessing(input_cloud, cloud, outdir, leafsize=2.00, cut=80):
	"""
	Gera base com pré-processamentos comuns. Obrigado, Open3D!
	
	:param input_cloud: caminho completo para uma nuvem
	:param cloud: apenas o nome da nuvem para fins de salvamento (bad code)
	:param outdir: caminho para pasta aonde a nuvem pré-processada será salva
	:param leafsize: leafsize do voxel grid (mm)
	:param outnn: número de vizinhos para decisão se ponto é outlier
	:param outstd: número de desvios-padrão para decisão se ponto é outlier
	:param cut: tamanho do corte ao redor do centro de massa (bad code)
	"""
	
	# carrega nuvem
	pcd = read_point_cloud(input_cloud)
	
	# downsample
	voxelpcd = voxel_down_sample(pcd, voxel_size=leafsize)
	
	# outlier removal
	outpcd, _ = radius_outlier_removal(voxelpcd, 50, 10)
	
	# nose tip pra corte!!!
	df = pd.DataFrame(np.asarray(outpcd.points))
	df.to_csv("../dump/temp.xyz", header=None, index=None, sep=' ')
	nosetip, normal = get_nosetip_normal("../dump/temp.xyz")
	
	# translada para nariz
	outpcd = np.asarray(outpcd.points)
	origin = np.array([0, 0, 0])
	direction = origin - nosetip
	transpcd = outpcd + direction
	
	"""
	# corte
	idx = linalg.norm(transpcd, axis=1) <= cut
	transpcd = transpcd[idx]
	"""
	
	"""
	# ajuste de pose usando PCA
	pca = PCA(n_components=3)
	pca.fit(transpcd)
	v = pca.components_
	"""
	
	x = np.array([1, 0, 0])
	y = np.array([0, 1, 0])
	#z = v[2]
	z = normal
	
	y = y * np.dot(z, y)
	x = np.cross(z, y)
	
	x = unit_vector(x)
	y = unit_vector(y)
	z = unit_vector(z)
	
	rot = np.absolute(np.array([x, y, z]))
	rotpcd = np.matmul(transpcd, rot)
	
	# salva nuvem
	df = pd.DataFrame(rotpcd)
	df.to_csv(outdir + cloud, header=None, index=None, sep=' ')


if __name__ == "__main__":
	folder = "../datasets/bosphorus/neutral/"
	outdir = "../datasets/tutu/neutral/"
	
	for cloud in os.listdir(folder):
		if ".xyz" in cloud:
			input_cloud = folder + cloud
			preprocessing(input_cloud, cloud, outdir)
			print(cloud, "ok!")
	















