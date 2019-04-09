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
                  leafsize=2.00, outnn=40, outstd=0.5, cut=80):
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
	outpcd, _ = statistical_outlier_removal(voxelpcd, outnn, outstd)
	
	# nose tip pra corte!!!
	df = pd.DataFrame(np.asarray(outpcd.points))
	df.to_csv("../dump/temp.xyz", header=None, index=None, sep=' ')
	cmd = ["../bin/nosext", "../dump/temp.xyz"]
	ans = subprocess.run(cmd, stdout=subprocess.PIPE).stdout
	l = ans[:-2].decode("utf-8").split('\n')
	normal = np.array([float(x) for x in l[0].split(' ')])
	nosetip = np.array([float(x) for x in l[1].split(' ')])
	
	# translada para nariz
	outpcd = np.asarray(outpcd.points)
	origin = np.array([0, 0, 0])
	direction = origin - nosetip
	transpcd = outpcd + direction
	
	# corte (em relacao ao centro de massa, mudar pro nariz depois)
	idx = linalg.norm(transpcd, axis=1) <= cut
	transpcd = transpcd[idx]
	
	# normal pra alinhamento!!!
	df = pd.DataFrame(transpcd)
	df.to_csv("../dump/temp.xyz", header=None, index=None, sep=' ')
	cmd = ["../bin/nosext", "../dump/temp.xyz"]
	ans = subprocess.run(cmd, stdout=subprocess.PIPE).stdout
	l = ans[:-2].decode("utf-8").split('\n')
	normal = np.array([float(x) for x in l[0].split(' ')])
	nosetip = np.array([float(x) for x in l[1].split(' ')])
	
	# ajuste de pose usando PCA (versao revisada e melhorada)
	pca = PCA(n_components=3)
	pca.fit(transpcd)
	v = pca.components_
	
	x = np.array([1, 0, 0])
	y = np.array([0, 1, 0])
	z = v[2]
	#z = normal
	
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
















