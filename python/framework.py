from dataset import *
from features import *
from classification import *

"""
zernike = "../results/frontal/bosphorus-outlier-densit200-crop80-icp/neutral-zernike.dat"
legendre = "../results/radial/bosphorus-outlier-densit200-crop80-icp/neutral-chebyshev.dat"
ans = rank1_concat([zernike, legendre]);
classifier, rate = max_rate(ans)
print("{} : {}".format(classifier, round(rate*100, 2)))
"""

cuts = {"f" : "frontal",
        "r" : "radial",
        "s" : "sagittal",
        "t" : "transversal",
        "w" : "whole"}

replace_dict = {"bosphorus" : "bs",
                "outlier" : "out",
                "densit" : "d",
                "crop" : "c"}

scenarios = ["bosphorus",
			 "bosphorus-outlier",
			 "bosphorus-outlier-densit200",
			 "bosphorus-outlier-densit200-crop60",
			 "bosphorus-outlier-densit200-crop70",
			 "bosphorus-outlier-densit200-crop80",
			 "bosphorus-outlier-densit200-crop60-icp",
	         "bosphorus-outlier-densit200-crop70-icp",
	         "bosphorus-outlier-densit200-crop80-icp",
			 "bosphorus-outlier-densit225",
			 "bosphorus-outlier-densit225-crop60",
			 "bosphorus-outlier-densit225-crop70",
			 "bosphorus-outlier-densit225-crop80",
	         "bosphorus-outlier-densit225-crop60-icp",
	         "bosphorus-outlier-densit225-crop70-icp",
	         "bosphorus-outlier-densit225-crop80-icp"]

moments = ["hu1980", "hututu", "legendre", "chebyshev", "zernike"]

datasets = ["../datasets/" + x + "/neutral" for x in scenarios]

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
			ans = rank1_neutral(moment, folder+"neutral-{}.dat".format(moment))
			classifier, rate = max_rate(ans)
			rounded = round(rate*100, 2)
			
			print("{:<11}{:<15}{:<7}".format(moment, classifier, rounded))
			row[moment] = rounded
		
		print()
		df = df.append(row, ignore_index=True)
	
	df.to_csv("../results/{}.csv".format(cut_folder), index=False)
"""



