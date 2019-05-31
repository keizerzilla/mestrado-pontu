from recognition import *

input_clouds = "../datasets/siqueira/clouds/"
input_lm3 = "../datasets/siqueira/lm3/"
output_clouds = "../datasets/siqueira-icp/clouds/"
output_lm3 = "../datasets/siqueira-icp/lm3/"
f_all = "../datasets/siqueira-icp/all/"

registration(input_clouds, input_lm3, output_clouds, output_lm3, f_all)

dataset = "../datasets/siqueira-icp/clouds/"
noses = "../datasets/siqueira-icp/lm3/"
result_folder = "../results/robson/"

os.makedirs(result_folder, exist_ok=True)

cuts = ["st"]
for cut in cuts:
	output = result_folder + cut + "_data.dat"
	
	batch_extraction(dataset, noses, cut, output)
	
	ans = rank_neutral(output)
	rocog_rate(ans, cut.upper() + "_neutral")
	
