import os
import subprocess
import numpy as np
import pandas as pd
import numpy.linalg as linalg
from open3d import *
from sklearn.decomposition import PCA

def unit_vector(v):
	return v / (v**2).sum()**0.5

def preprocessing(input_cloud, cloud, outdir,
                  leafsize=2.00, outnn=10, outstd=1.00, cut=100):
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
	
	# outlier removal
	pcd, _ = statistical_outlier_removal(pcd, outnn, outstd)
	
	# downsample
	pcd = voxel_down_sample(pcd, voxel_size=leafsize)
	
	# nose tip pra corte!!!
	df = pd.DataFrame(np.asarray(pcd.points))
	df.to_csv("../dump/temp.xyz", header=None, index=None, sep=' ')
	cmd = ["../bin/nosext", "../dump/temp.xyz"]
	ans = subprocess.run(cmd, stdout=subprocess.PIPE).stdout
	l = ans[:-2].decode("utf-8").split('\n')
	normal = np.array([float(x) for x in l[0].split(' ')])
	nosetip = np.array([float(x) for x in l[1].split(' ')])
	
	# translada para centro
	pcd = np.asarray(pcd.points)
	origin = np.array([0, 0, 0])
	direction = origin - nosetip
	pcd = pcd + direction
	
	# corte (em relacao ao centro de massa, mudar pro nariz depois)
	idx = linalg.norm(pcd, axis=1) <= cut
	pcd = pcd[idx]
	
	# normal pra alinhamento!!!
	df = pd.DataFrame(pcd)
	df.to_csv("temp.xyz", header=None, index=None, sep=' ')
	cmd = ["../bin/nosext", "temp.xyz"]
	ans = subprocess.run(cmd, stdout=subprocess.PIPE).stdout
	l = ans[:-2].decode("utf-8").split('\n')
	normal = np.array([float(x) for x in l[0].split(' ')])
	nosetip = np.array([float(x) for x in l[1].split(' ')])
	
	# ajuste de pose usando PCA (versao revisada e melhorada)
	"""
	pca = PCA(n_components=3)
	pca.fit(pcd)
	v = pca.components_
	"""
	
	x = np.array([1, 0, 0])
	y = np.array([0, 1, 0])
	#z = np.absolute(v[2])
	z = normal
	
	y = y * np.dot(z, y)
	x = np.cross(z, y)
	
	x = unit_vector(x)
	y = unit_vector(y)
	z = unit_vector(z)
	
	rot = np.absolute(np.array([x, y, z]))
	pcd = np.matmul(pcd, rot)
	
	# salva nuvem
	df = pd.DataFrame(pcd)
	df.to_csv(outdir + cloud, header=None, index=None, sep=' ')


if __name__ == "__main__":
	folder = "../datasets/bosphorus-outlier/neutral/"
	outdir = "../datasets/tutu/neutral/"
	
	for cloud in os.listdir(folder):
		if ".xyz" in cloud:
			input_cloud = folder + cloud
			preprocessing(input_cloud, cloud, outdir)
			print(cloud, "ok!")
















