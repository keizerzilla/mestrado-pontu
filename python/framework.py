import sys
from features import *
from classification import *

replace_dict = {"bosphorus" : "bs",
                "outlier"   : "out",
                "densit"    : "d",
                "crop"      : "c"}

faces = ["neutral", "happy", "occlusion", "nonneutral"]

cuts = {"7" : "seven"}

scenarios= ["bosphorus-pdi"]

moments = ["legendre", "chebyshev"]

def go_classification_rank1(rdir, tname="Rank1"):
	for face in faces:
		datasets = ["../datasets/" + x + "/{}".format(face) for x in scenarios]
		for cut, cut_folder in cuts.items():
			
			for data in datasets:
				scenario = data.split("/")[2]
				folder = "../{}/{}/{}/".format(rdir, cut_folder, scenario)
				
				print("[{}] - [{}]".format(cut_folder, folder))
				
				dataset = scenario
				for old, new in replace_dict.items():
					dataset = dataset.replace(old, new)
				
				for moment in moments:
					neutral = folder + "neutral-{}.dat".format(moment)
					
					ans = None
					if face == "neutral":
						ans = rank1_neutral(neutral)
					else:
						nonneutral = folder + "{}-{}.dat".format(face, moment)
						ans = rank1_nonneutral(neutral, nonneutral)
					
					summary(ans, moment)
					#classifier, rate = max_rate(ans)
					#rate = round(rate*100, 2)
					#print("{:<11}{:<15}{:<7}".format(moment, classifier, rate))

def go_classification_roc1(rdir):
	for cut, cut_folder in cuts.items():
		for scenario in scenarios:
			folder = "../{}/{}/{}/".format(rdir, cut_folder, scenario)
			
			print("[{}] - [{}]".format(cut_folder, folder))
			
			dataset = scenario
			for old, new in replace_dict.items():
				dataset = dataset.replace(old, new)
			
			for moment in moments:
				neutral = folder + "neutral-{}.dat".format(moment)
				nonneutral = folder + "nonneutral-{}.dat".format(moment)
				
				ans = roc1(neutral, nonneutral)
				classifier, rate = max_rate(ans)
				rate = round(rate*100, 2)
				print("{:<11}{:<15}{:<7}".format(moment, classifier, rate))

if __name__ == "__main__":
	#total_extraction(faces, scenarios, cuts, moments, "pdi")
	go_classification_rank1("pdi")
	#go_classification_roc1("tutu")
	#go_combination(n=3)
	
