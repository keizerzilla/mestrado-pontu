import os
import numpy as np
from open3d import *

def dispersion_analysis(cloud_file):
	# carrega nuvem
	pcd = read_point_cloud(cloud_file)
	pcd_data = np.asarray(pcd.points)

	# translada para centro
	centroid = np.mean(pcd_data, axis=0)
	origin = np.array([0, 0, 0])
	direction = origin - centroid
	pcd_trans = pcd_data + direction
	translated = PointCloud()
	translated.points = Vector3dVector(pcd_trans)

	cov = np.cov(pcd_trans.T)
	w, v = np.linalg.eig(cov)
	idx = w.argsort()[::1]   
	w = w[idx]
	v = v[:,idx]

	f = 20

	points = [[0, 0, 0]] + [list(p*f) for p in v]
	lines = [[0, 1], [0, 2], [0, 3]]
	colors = [[1, 0, 0], [0, 1, 0], [0, 0, 1]]
	dispersion_axis = LineSet()
	dispersion_axis.points = Vector3dVector(points)
	dispersion_axis.lines = Vector2iVector(lines)
	dispersion_axis.colors = Vector3dVector(colors)

	mesh_frame = create_mesh_coordinate_frame(size=f)

	draw_geometries([translated, dispersion_axis, mesh_frame])

directory = "../datasets/bosphorus-outlier/neutral/"

for f in os.listdir(directory):
	if ".xyz" in f:
		cloud_file = directory + f
		dispersion_analysis(cloud_file)

