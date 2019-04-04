import os
import subprocess
import numpy as np
from open3d import *

def dispersion_analysis(cloud_file, direction):
	# carrega nuvem
	pcd = read_point_cloud(cloud_file)
	pcd_data = np.asarray(pcd.points)
	
	f = 20
	
	d = np.array(direction)*f
	
	points = [[0, 0, 0], d]
	lines = [[0, 1]]
	colors = [[1, 0, 0]]
	dispersion_axis = LineSet()
	dispersion_axis.points = Vector3dVector(points)
	dispersion_axis.lines = Vector2iVector(lines)
	dispersion_axis.colors = Vector3dVector(colors)

	mesh_frame = create_mesh_coordinate_frame(size=f)

	draw_geometries([pcd, dispersion_axis, mesh_frame])

if __name__ == "__main__":
	directory = "../datasets/bosphorus-outlier-densit225-crop80/"
	for f in os.listdir(directory):
		if ".xyz" in f:
			cloud_file = directory + f
			
			cmd = ["./demo", cloud_file]
			ans = subprocess.run(cmd, stdout=subprocess.PIPE).stdout
			ans = ans[:-2].decode("utf-8")
			ans = [float(x) for x in ans.split()]
			
			print(f, ans)
			dispersion_analysis(cloud_file, ans)
			
