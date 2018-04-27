import os
import subprocess

datafolders = ["../nonneutral/"]
outputdir = "../centerprofile/"

for folder in datafolders:
	for cloud in os.listdir(folder):
		filename = cloud.split(".")[0]
		cloud = folder + cloud
		subprocess.run(["../../bin/cloudz", cloud, outputdir + filename + ".txt"])
		
print("TERMINADO")
