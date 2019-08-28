import os
import subprocess

dataset = "../datasets/SNCV2/"
bindir = "../pontu/bin/shapenet"
modelFile = "models/model_normalized.obj"
moments = ["sm", "zo", "ze", "zm", "zf"]

count = 0
for moment in moments:
	dumpPath = "../results/shapenet_{}.dat".format(moment)
	with open(dumpPath, "w") as dump:
		for shape in os.listdir(dataset):
			objectPath = os.path.join(dataset, shape)
			
			if not os.path.isdir(objectPath):
				continue
			
			for sample in os.listdir(objectPath):
				samplePath = os.path.join(objectPath, sample)
				
				if not os.path.isdir(samplePath):
					continue
				
				samplePath = os.path.join(samplePath, modelFile)
				
				if not os.path.isfile(samplePath):
					continue
				
				cmd = [bindir, samplePath, moment]
				ans = subprocess.run(cmd, stdout=subprocess.PIPE).stdout
				ans = ans[:-1].decode("utf-8").replace(" ", ",")
				ans = ans + ",{},{}\n".format(sample, shape)
				dump.write(ans)
				
				count = count + 1
				print(moment, sample, "ok!", count)
				
