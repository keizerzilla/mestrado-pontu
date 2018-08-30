import sys
import os
import subprocess
import parse
import time

folder = "../datasets/bosphorus/neutrals-free/"
moments = "../dump/hu-moments-neutrals-free-train.dat"
classes = "../dump/hu-classes-neutrals-free-train.dat"
parse_fmt = "bs{:d}_{}_{}_{:w}.cloud"
hard_mode = False
class_file = open(classes, "w")

for cloud in os.listdir(folder):
	if cloud.endswith(".cloud"):
		sample_class = str(parse.parse(parse_fmt, cloud)[0])
		
		if hard_mode:
			sample_index = str(parse.parse(parse_fmt, cloud)[3])
			if sample_index != "0":
				continue
		
		subprocess.call(["../bin/mcalc", "-m", "hu", "-i", folder+cloud, "-o", moments])
		class_file.write(str(sample_class) + "\n")
		print("{} OK".format(cloud))

class_file.close()
print("Pronto!")
