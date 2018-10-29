from dataset import *
from features import *
from classification import *

scenarios = ["bosphorus-outlier-densit200-crop60-icp",
	         "bosphorus-outlier-densit200-crop70-icp",
	         "bosphorus-outlier-densit200-crop80-icp",
	         "bosphorus-outlier-densit225-crop60-icp",
	         "bosphorus-outlier-densit225-crop70-icp",
	         "bosphorus-outlier-densit225-crop80-icp",]
moments = ["zernike", "legendre", "hu1980", "chebyshev", "hututu"]
datasets = ["../datasets/" + x + "/neutral" for x in scenarios]
cuts = ["r"]

"""
for data in datasets:
	for cut in cuts:
		scenario = data.split("/")[2]
		folder = "../results/{}-cut_{}".format(scenario, cut)
		try:
			os.mkdir(folder)
		except:
			print("OPS! Diretorio jah existe!")
		
		for moment in moments:
			out = folder
			out = out + "/{}-{}.dat".format(os.path.split(data)[1], moment)
			moment_extraction_batch(moment, cut, data, out)

"""
for data in datasets:
	for cut in cuts:
		scenario = data.split("/")[2]
		folder = "../results/{}-cut_{}".format(scenario, cut)
		for moment in moments:
			print(folder)
			rank1_neutral(moment, folder + "/neutral-{}.dat".format(moment))


