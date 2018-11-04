from dataset import *
from features import *
from classification import *
from pathlib import Path

face = "nonneutral"

cuts = {"f" : "frontal",
        "r" : "radial",
        "s" : "sagittal",
        "t" : "transversal",
        "w" : "whole"}

replace_dict = {"bosphorus" : "bs",
                "outlier" : "out",
                "densit" : "d",
                "crop" : "c"}

scenarios = ["bosphorus-outlier-densit200-crop60-icp",
	         "bosphorus-outlier-densit200-crop70-icp",
	         "bosphorus-outlier-densit200-crop80-icp",
	         "bosphorus-outlier-densit225-crop60-icp",
	         "bosphorus-outlier-densit225-crop70-icp",
	         "bosphorus-outlier-densit225-crop80-icp"]

moments = ["hu1980", "hututu", "legendre", "chebyshev", "zernike"]

datasets = ["../datasets/" + x + "/{}".format(face) for x in scenarios]

"""
for cut, cut_folder in cuts.items():
	for data in datasets:
		scenario = data.split("/")[2]
		folder = "../results/{}/{}/".format(cut_folder, scenario)
		
		try:
			Path(folder).mkdir(parents=True, exist_ok=True)
		except:
			print("OPS! Diretorio jah existe!")
		
		for moment in moments:
			out = folder
			out = out + "{}-{}.dat".format(face, moment)
			moment_extraction_batch(moment, cut, data, out)
"""


cols = ["dataset"] + moments
for cut, cut_folder in cuts.items():
	df = pd.DataFrame(columns=cols)
	row = dict()
	
	for data in datasets:
		scenario = data.split("/")[2]
		folder = "../results/{}/{}/".format(cut_folder, scenario)
		
		print(folder)
		
		dataset = scenario
		for old, new in replace_dict.items():
			dataset = dataset.replace(old, new)
		
		row["dataset"] = dataset
		
		for moment in moments:
			neutral = folder + "neutral-{}.dat".format(moment)
			nonneutral = folder + "nonneutral-{}.dat".format(moment)
			#ans = rank1_neutral(moment, folder+"{}-{}.dat".format(face, moment))
			ans = rank1_nonneutral(moment, neutral, nonneutral)
			classifier, rate = max_rate(ans)
			rounded = round(rate*100, 2)
			
			print("{:<11}{:<15}{:<7}".format(moment, classifier, rounded))
			row[moment] = rounded
		
		print()
		df = df.append(row, ignore_index=True)
	
	#df.to_csv("../results/{}-{}.csv".format(face, cut_folder), index=False)


