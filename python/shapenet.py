import os
import subprocess
import numpy as np
import pandas as pd

dataset = "/media/kz1/TUTU640GB/ShapeNetCoreV2/"
bindir = "../pontu/bin/shapenet"
dumpPath = "../shapenet_spheric.dat"

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
			
			cmd = [bindir, samplePath]
			ans = subprocess.run(cmd, stdout=subprocess.PIPE).stdout
			ans = ans[:-1].decode("utf-8").replace(" ", ",")
			ans = ans + ",{},{}\n".format(sample, shape)
			dump.write(ans)
			
			print(sample, shape, "ok!")
			
