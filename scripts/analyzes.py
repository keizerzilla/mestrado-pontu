import os
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

def analyzes(src, cloud, dst):
	path = src + cloud
	df = np.array(pd.read_csv(path, header=None))
	df = np.sort(df, axis=0)
	np.savetxt(dst+cloud, df)
	print(path + " OK")

def batchanalyzes():
	src = "/home/artur/pesquisa/centerprofile/nonneutral/"
	dst = "/home/artur/pesquisa/centerprofile/nonneutral_sort/"

	for cloud in os.listdir(src):
		if cloud.endswith("txt"):
			#path = src + cloud
			#df = np.array(pd.read_csv(path, header=None))
			#df = np.sort(df, axis=0)
			#np.savetxt(dst+cloud, df)
			#print(path + " OK")
			analyzes(src, cloud, dst)
