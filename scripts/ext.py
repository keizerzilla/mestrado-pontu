import sys
import os
import subprocess
import parse
import time

folder = "../datasets/bosphorus/neutrals-free/"
moments = "../dump/hu-moments-neutrals-free.dat"
classes = "../dump/hu-classes-neutrals-free.dat"
parse_fmt = "bs{:d}_{}_{}_{:w}.cloud"

class_file = open(classes, "w")

for cloud in os.listdir(folder):
	if cloud.endswith(".cloud"):
		sample_class = str(parse.parse(parse_fmt, cloud)[0])
		subprocess.call(["../bin/mcalc", "-m", "hu", "-i", folder+cloud, "-o", moments])
		class_file.write(str(sample_class) + "\n")
		print("{} OK".format(cloud))

class_file.close()
print("Pronto!")
