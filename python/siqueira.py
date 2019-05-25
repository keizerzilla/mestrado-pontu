from recognition import *

"""----------------
TAREFA DE REGISTRO
----------------"""

input_clouds = "../datasets/tutu/clouds/"
input_lm3 = "../datasets/tutu/lm3/"
output_clouds = "../datasets/tutu_icp/clouds/"
output_lm3 = "../datasets/tutu_icp/lm3/"

#registration(input_clouds, input_lm3, output_clouds, output_lm3)

"""----------------------
EXTRACAO E CLASSIFICACAO
----------------------"""

dataset = "../datasets/siqueira/clouds/"
noses = "../datasets/siqueira/lm3/"
result_folder = "../results/siqueira/"

os.makedirs(result_folder, exist_ok=True)

cuts = ["f", "s", "t", "fs", "ft", "st", "fst"]
for cut in cuts:
	output = result_folder + cut + "_data.dat"
	
	#batch_extraction(dataset, noses, cut, output)
	
	ans1 = rank_neutral(output)
	max_recognition(ans1, cut.upper() + "_neutral")
	
	ans2 = rank_nonneutral(output)
	max_recognition(ans2, cut.upper() + "_nonneutral")

