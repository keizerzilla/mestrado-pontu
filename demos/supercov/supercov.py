
import numpy as np
import numpy.linalg as linalg
import pandas as pd
from mpl_toolkits import mplot3d
import matplotlib.pyplot as plt

clouds = {
	"raw" : "../../datasets/bosphorus/neutral/bs000_N_N_0.xyz",
	"regular" : "../../datasets/bosphorus-outlier-densit225/neutral/bs000_N_N_0.xyz",
	"registred" : "../../datasets/bosphorus-outlier-densit225-crop80-icp/neutral/bs000_N_N_0.xyz"
}

for index, cloud in clouds.items():
	df = pd.read_csv(cloud, header=None, sep=" ", names=["x", "y", "z"])
	xdata = np.array(df["x"])
	ydata = np.array(df["y"])
	zdata = np.array(df["z"])
	points = np.array(df).T
	cov = np.cov(points)
	w, v = linalg.eig(cov)
	
	fig = plt.figure()
	ax = fig.add_subplot(111, projection="3d")
	ax.scatter3D(xdata, ydata, zdata, c=zdata, cmap="Purples", s=1)
	
	origin = [0, 0, 0]
	print(origin)
	for i in range(3):
		xv = [origin[0], v[i][0]*32]
		yv = [origin[1], v[i][1]*32]
		zv = [origin[2], v[i][2]*32]
		ax.plot3D(xv, yv, zv)
	
	ax.set_xlabel("X")
	ax.set_ylabel("Y")
	ax.set_zlabel("Z")
	plt.title(index)
	plt.show()

