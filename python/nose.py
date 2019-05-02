import os
import sys
import parse
import subprocess
import numpy as np
import pandas as pd
import numpy.linalg as linalg
import matplotlib.pyplot as plt

def nose_bosphorus(folder):
	fmt = "bs{:d}_{:w}_{:w}_{:d}.xyz"
	vparse = "{:w} {:w} {:w}"
	
	nose_folder = folder + "nose/"
	lm3_folder = os.path.dirname(os.path.dirname(folder)) + "/LM3/"
	
	dump = {"cloud" : [], "groundtruth" : [], "tutu_found" : [], "dist" : []}
	
	parts = folder.split("/")
	f = os.path.dirname(os.path.dirname(folder)) + "/nose-" + parts[3] + ".csv"
	
	bsnose = folder + "bsnose/"
	os.makedirs(bsnose, exist_ok=True)
	
	for i in os.listdir(nose_folder):
		if ".xyz" in i:
			match = parse.parse(fmt, i)
			subject = "bs{:03d}".format(match[0])
			bslm3 = lm3_folder + subject + "/" + i.replace("xyz", "lm3")
			
			with open(bslm3, "r") as lm3:
				content = [line.rstrip('\n') for line in lm3]
				try:
					nose = nose_folder + i
					idx = content.index("Nose tip")
					points = [float(a) for a in content[idx+1].split(" ")]
					tutu = open(nose, "r").read().rstrip('\n')
					tutu_points = [float(a) for a in tutu.split(" ")]
					
					df1 = np.array([tutu_points])
					df2 = np.array([points])
					d = linalg.norm(df1 - df2)
					
					dump["cloud"].append(i)
					dump["groundtruth"].append(points)
					dump["tutu_found"].append(tutu_points)
					dump["dist"].append(d)
					
					bs_nosetip = {"x" : [points[0]],
					              "y" : [points[1]],
					              "z" : [points[2]]}
					
					outfile = bsnose + i
					ans = pd.DataFrame(bs_nosetip)
					ans.to_csv(outfile, header=None, index=None, sep=' ')
				except:
					dump["cloud"].append(i)
					dump["groundtruth"].append([0, 0, 0])
					dump["tutu_found"].append([0, 0, 0])
					dump["dist"].append(np.nan)
					
					print("default nose not found at", i)
				
	saved = pd.DataFrame(dump)
	mean = saved["dist"].mean()
	std = saved["dist"].std()
	print("MEDIA: {}, DESVIO: {}".format(mean, std))
	saved.to_csv(f, index=None)
	
def nose_dispersion(datafile):
	df = pd.read_csv(datafile)
	gt = list(df.groundtruth)
	fd = list(df.tutu_found)
	
	coords = {"x_gt" : [], "y_gt" : [], "z_gt" : [],
	          "x_fd" : [], "y_fd" : [], "z_fd" : [],
	          "delta_x" : [], "delta_y" : [], "delta_z" : []}
	
	for g, f in zip(gt, fd):
		lg = g.replace("[", "").replace("]", "").split(",")
		lg = [float(x) for x in lg]
		coords["x_gt"].append(lg[0])
		coords["y_gt"].append(lg[1])
		coords["z_gt"].append(lg[2])
		
		lf = f.replace("[", "").replace("]", "").split(",")
		lf = [float(x) for x in lf]
		coords["x_fd"].append(lf[0])
		coords["y_fd"].append(lf[1])
		coords["z_fd"].append(lf[2])
		
		coords["delta_x"].append(lg[0] - lf[0])
		coords["delta_y"].append(lg[1] - lf[1])
		coords["delta_z"].append(lg[2] - lf[2])
		
	coords = pd.DataFrame(coords)
	
	plt.subplot(1, 4, 1)
	coords["delta_x"].hist()
	plt.title("ERRO X")
	
	plt.subplot(1, 4, 2)
	coords["delta_y"].hist()
	plt.title("ERRO Y")
	
	plt.subplot(1, 4, 3)
	coords["delta_z"].hist()
	plt.title("ERRO Z")
	
	plt.subplot(1, 4, 4)
	df["dist"].hist()
	plt.title("ERRO DIST")
	
	plt.show()
	
def nose_extraction(folder):
	output = folder + "nose/"
	os.makedirs(output, exist_ok=True)
	
	for cloud in os.listdir(folder):
		if ".xyz" in cloud:
			input_cloud = folder + cloud
			output_cloud = output + cloud
			
			cmd = ["../bin/nosex", input_cloud, output_cloud]
			subprocess.run(cmd)
			
			print(input_cloud, "extracted ok!")

def nose_convert_pcd(folder):
	for cloud in os.listdir(folder):
		if ".xyz" in cloud:
			output_cloud = folder + cloud
			output_pcd = output_cloud.replace(".xyz", ".pcd")
			
			convert = ["pcl_xyz2pcd", output_cloud, output_pcd]
			subprocess.run(convert)

def nose_view(folder):
	output = folder + "nose/"
	
	for cloud in os.listdir(folder):
		if ".xyz" in cloud:
			input_cloud = folder + cloud
			output_cloud = output + cloud
			input_pcd = input_cloud.replace(".xyz", ".pcd")
			output_pcd = output_cloud.replace(".xyz", ".pcd")
			
			view = ["pcl_viewer", "-fc", "0,0,255", "-ps", "2", input_pcd,
			                      "-fc", "255,0,0", "-ps", "5", output_pcd]
			subprocess.run(view)

if __name__ == "__main__":
	outdir = "../datasets/bosphorus-tutu/neutral/"
	
	nose_extraction(outdir)
	nose_bosphorus(outdir)
	nose_dispersion("../datasets/bosphorus-tutu/nose-neutral.csv")
	
	#nose_convert_pcd(outdir)
	nose_convert_pcd(outdir + "nose/")
	#nose_convert_pcd(outdir + "bsnose/")
	#nose_view(outdir)
	
