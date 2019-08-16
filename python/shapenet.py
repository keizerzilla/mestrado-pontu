import os
import sys
import subprocess
import numpy as np
import pandas as pd

if len(sys.argv) != 2:
	print("python: numero de parametros incorretos!")
	print("python: uso     -> python3 shapenet.py <tipo_de_zernike>")
	print("python: tipos   -> odd, even, mag")
	print("python: exemplo -> python3 shapenet.py odd")
	sys.exit(1)

dataset = "/home/keizerzilla/SNCV2/"
bindir = "../pontu/bin/shapenet"
dumpPath = "../results/shapenet_zernike_{}.dat".format(sys.argv[1])

with open(dumpPath, "w") as dump:
	for shape in os.listdir(dataset):
		objectPath = os.path.join(dataset, shape)
		
		if not os.path.isdir(objectPath):
			continue
		
		for sample in os.listdir(objectPath):
			samplePath = os.path.join(objectPath, sample)
			
			if not os.path.isdir(samplePath):
				continue
			
			samplePath = os.path.join(samplePath, "models/model_normalized.obj")
			
			if not os.path.isfile(samplePath):
				continue
			
			cmd = [bindir, samplePath, sys.argv[1]]
			ans = subprocess.run(cmd, stdout=subprocess.PIPE).stdout
			ans = ans[:-1].decode("utf-8").replace(" ", ",")
			ans = ans + ",{},{}\n".format(sample, shape)
			dump.write(ans)
			
			print(sample, shape, "ok!")
			
