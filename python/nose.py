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
					
					print(points, tutu_points, d)
				except:
					dump["cloud"].append(i)
					dump["groundtruth"].append([0, 0, 0])
					dump["tutu_found"].append([0, 0, 0])
					dump["dist"].append(np.nan)
					
					print("not found at", i)
	
	saved = pd.DataFrame(dump)
	mean = saved["dist"].mean()
	std = saved["dist"].std()
	
	saved.hist("dist", bins=10)
	plt.show()
	
	
	print("MEDIA: {}, DESVIO: {}".format(mean, std))
	
	saved.to_csv(f, index=None)
	
	
def nose_extraction(folder):
	output = folder + "nose/"
	os.makedirs(output, exist_ok=True)
	
	for cloud in os.listdir(folder):
		if ".xyz" in cloud:
			input_cloud = folder + cloud
			output_cloud = output + cloud
			
			cmd = ["../bin/nosext", input_cloud, output_cloud]
			subprocess.run(cmd)

def nose_convert_pcd(folder):
	output = folder + "nose/"
	
	for cloud in os.listdir(folder):
		if ".xyz" in cloud:
			input_cloud = folder + cloud
			output_cloud = output + cloud
			input_pcd = input_cloud.replace(".xyz", ".pcd")
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
	#nose_convert_pcd("../datasets/tutu/neutral/")
	#nose_view("../datasets/tutu/neutral/")
	nose_extraction("../datasets/bosphorus-outlier/neutral/")
	nose_bosphorus("../datasets/bosphorus-outlier/neutral/")
	
