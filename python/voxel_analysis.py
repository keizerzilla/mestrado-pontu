import os
import time
import pandas as pd
import numpy as np
from open3d import *

folder = "../datasets/bosphorus/all/"

dump = {
	"filename" : [],
	"leafsize" : [],
	"points"   : [],
	"time"     : []
}

for f in os.listdir(folder):
	if ".pcd" in f:
		for leafsize in [0.25, 0.50, 1.00, 1.25, 1.50, 2.00]:
			cloud = folder + f
			
			pcd = read_point_cloud(cloud)
			start = time.time()
			downpcd = voxel_down_sample(pcd, voxel_size=leafsize)
			elapsed = time.time() - start
			points = len(downpcd.points)
			
			dump["filename"].append(f)
			dump["leafsize"].append(leafsize)
			dump["points"].append(points)
			dump["time"].append(elapsed)
			
			print(cloud, "OK!")

df = pd.DataFrame(dump)
df.to_csv("voxel_open3d.csv", index=None)

