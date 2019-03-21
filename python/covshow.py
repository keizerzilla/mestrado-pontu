import os
import numpy as np
import numpy.linalg as linalg
import pandas as pd

def angle_between(v1, v2):
	return np.degrees(np.arccos(np.clip(np.dot(v1, v2), -1.0, 1.0)))

neutrals = []
with open("res/neutralfiles.txt", "r") as content_neutral:
	neutrals = content_neutral.read().split()

folder = "../datasets/bosphorus-outlier/neutral/"

ans = {"cloud" : [], "minvector" : [], "alphaz" : []}
x = np.array([1, 0, 0])
y = np.array([0, 1, 0])
z = np.array([0, 0, 1])
for cloud in neutrals:
	fpath = folder + cloud + ".xyz"
	df = pd.read_csv(fpath, header=None, sep=' ')
	cov = df.cov()
	w, v = linalg.eig(cov)
	
	idx = w.argsort()[::1]
	w = w[idx]
	v = v[:,idx]
	
	alphax = angle_between(v[0], x)
	alphay = angle_between(v[0], y)
	alphaz = angle_between(v[0], z)
	
	ans["cloud"].append(cloud)
	ans["minvector"].append(v[0])
	ans["alphaz"].append(alphaz)
	
	print("{}\tx: {}\ty: {}\tz: {}".format(cloud, alphax, alphay, alphaz))

