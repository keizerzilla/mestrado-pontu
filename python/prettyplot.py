import sys
import pandas as pd
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

if len(sys.argv) != 3:
	print("error")
	exit()

cloud1 = pd.read_csv(sys.argv[1], sep=" ", header=None, names=["x", "y", "z"])
cloud2 = sys.argv[2]

fig = plt.figure()
ax = fig.add_subplot(111, projection="3d")
plt.scatter(cloud1.x.tolist(), cloud1.y.tolist(), cloud1.z.tolist())

ax.set_xlabel('X Label')
ax.set_ylabel('Y Label')
ax.set_zlabel('Z Label')

plt.show()

