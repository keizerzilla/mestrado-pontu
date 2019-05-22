from recognition import *

input_folder = "../datasets/tutu/BOSPHORUS_clouds/"
nose_folder = "../datasets/tutu/BOSPHORUS_lm3/"
output_folder = "../datasets/tutu/BOSPHORUS_icp/"

os.makedirs(output_folder, exist_ok=True)

registration(input_folder, nose_folder, output_folder)

"""
dataset = "../datasets/siqueira/BOSPHORUS_clouds/"
noses = "../datasets/siqueira/BOSPHORUS_lm3/"
cuts = ["f", "s", "t", "fs", "ft", "st", "fst"]
result_folder = "../results/siqueira/"

os.makedirs(result_folder, exist_ok=True)

for cut in cuts:
	output = result_folder + cut + "_data.dat"
	batch_extraction(dataset, noses, cut, output)
	ans = rank_neutral(output)
	max_recognition(ans, cut.upper() + "_neutral")
"""

