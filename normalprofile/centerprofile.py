import os
import subprocess

# FUNCAO SHOW #01
def centerprofile(folder, cloud, outputdir):
	print("PROCESSANDO " + cloud)
	filename = cloud.split(".")[0]
	cloud = folder + cloud
	subprocess.run(["../bin/cloudz", cloud, outputdir + filename + ".txt"])

# FUNCAO SHOW #02
def batchprofile():
	datafolders = ["../nonneutral/"]
	outputdir = "../centerprofile/"

	for folder in datafolders:
		for cloud in os.listdir(folder):
			#filename = cloud.split(".")[0]
			#cloud = folder + cloud
			#subprocess.run(["../../bin/cloudz", cloud, outputdir + filename + ".txt"])
			centerprofile(folder, cloud, outputdir)
		
	print("TERMINADO")

centerprofile("../", "buddha.xyz", "../centerprofile/")
centerprofile("../", "bunny.xyz", "../centerprofile/")
centerprofile("../", "armadillo.xyz", "../centerprofile/")
centerprofile("../", "lucy.xyz", "../centerprofile/")
centerprofile("../", "head.xyz", "../centerprofile/")
