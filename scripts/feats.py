import sys
import os
import subprocess
import parse
import time

folder = "../datasets/Neutrals/"
output = "../results/hu1980-Neutrals.dat"
parse_fmt = "bs{:d}_{}_{}_{:w}.cloud"
hard_mode = False

for cloud in os.listdir(folder):
	if cloud.endswith(".cloud"):
		sample_class = str(parse.parse(parse_fmt, cloud)[0])
		sample_index = str(parse.parse(parse_fmt, cloud)[3])
		
		subprocess.run(["../bin/mcalc", "-m", "hu1980", "-c", folder+cloud, "-o", "stdout"])
		
		
		
		#class_file.write(str(sample_class) + "\n")
		#print("{} OK".format(cloud))
		#ret = ret + str(sample_class)
		#print(sample_class)
		#output.write(ret.stdout + str(sample_class))

#class_file.close()
#print("Pronto!")
