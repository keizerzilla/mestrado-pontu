import os
import numpy as np
import pandas as pd
import numpy.linalg as linalg
from open3d import *
from sklearn.decomposition import PCA

def unit_vector(v):
	return v / (v**2).sum()**0.5

def preprocessing(input_cloud, cloud, outdir,
                  leafsize=2.00, outnn=20, outstd=2.00, cut=80):
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
	pcd = voxel_down_sample(pcd, voxel_size=leafsize)
	
	# outlier removal
	pcd, _ = statistical_outlier_removal(pcd,
	                                     nb_neighbors=outnn,
	                                     std_ratio=outstd)
	
	# translada para centro
	pcd = np.asarray(pcd.points)
	centroid = np.mean(pcd, axis=0)
	origin = np.array([0, 0, 0])
	direction = origin - centroid
	pcd = pcd + direction
	
	# ajuste de pose usando PCA (versao revisada e melhorada)
	pca = PCA(n_components=3)
	pca.fit(pcd)
	v = pca.components_
	
	x = np.array([1, 0, 0])
	y = np.array([0, 1, 0])
	z = v[2]
	
	x = x * np.dot(z, x)
	y = np.cross(z, x)
	
	x = unit_vector(x)
	y = unit_vector(y)
	z = unit_vector(z)
	
	rot = np.absolute(np.array([x, y, z]))
	pcd = np.matmul(pcd, rot)
	
	# corte (em relacao ao centro de massa, mudar pro nariz depois)
	idx = linalg.norm(pcd, axis=1) <= cut
	pcd = pcd[idx]
	
	# salva nuvem
	df = pd.DataFrame(pcd)
	df.to_csv(outdir + cloud, header=None, index=None, sep=' ')

