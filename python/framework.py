from features import *
from classification import *

replace_dict = {"bosphorus" : "bs",
                "outlier"   : "out",
                "densit"    : "d",
                "crop"      : "c"}

faces = ["neutral"]

cuts = {"s" : "sagittal"}

scenarios= ["bosphorus-outlier-densit200-crop80-icp"]

moments = ["spheric"]

mini_scenarios = []
for s in scenarios:
	temp = s
	for old, new in replace_dict.items():
		temp = temp.replace(old, new)
	mini_scenarios.append(temp)

def plot_classification(res, title, rdir, cut_folder):
	ax = res.plot(marker='o', figsize=(11, 9))
	tick_marks_x = np.arange(len(mini_scenarios))
	tick_marks_y = range(0, 110, 10)
	plt.xticks(tick_marks_x, mini_scenarios, rotation=90)
	plt.yticks(tick_marks_y)
	filename = "{} - {}".format(title, cut_folder)
	plt.title(filename)
	plt.ylabel("Acerto (%)")
	plt.grid()
	
	for moment in moments:
		for x, y in enumerate(res[moment]):
			ax.annotate(str(y), xy=(x, y), xytext=(x-0.2, y+1))
	
	plt.tight_layout()
	figpath = "../" + rdir + "/figures/"
	os.makedirs(figpath, exist_ok=True)
	plt.savefig(figpath + filename + ".png")

def go_classification_rank1(rdir, dump=False, plot=False):
	for face in faces:
		datasets = ["../datasets/" + x + "/{}".format(face) for x in scenarios]
		for cut, cut_folder in cuts.items():
			
			res = pd.DataFrame(columns=moments, index=mini_scenarios)
			test_name = ""
			
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
						test_name = "Rank1a"
					else:
						nonneutral = folder + "nonneutral-{}.dat".format(moment)
						ans = rank1_nonneutral(neutral, nonneutral)
						test_name = "Rank1b"
					
					classifier, rate = max_rate(ans)
					rate = round(rate*100, 2)
					res.at[dataset, moment] = rate
					print("{:<11}{:<15}{:<7}".format(moment, classifier, rate))
			
			if dump:
				dumpfile = "../"+rdir+"/{}-{}-{}.csv".format(test_name, rdir, cut_folder)
				res.to_csv(dumpfile)
			if plot:
				plot_classification(res, test_name, rdir, cut_folder)

def go_classification_roc1(rdir, plot=False):
	for cut, cut_folder in cuts.items():
		res = pd.DataFrame(columns=moments, index=mini_scenarios)
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
				res.at[dataset, moment] = rate
				print("{:<11}{:<15}{:<7}".format(moment, classifier, rate))
		
		if plot:
			plot_classification(res, "ROC1", rdir, cut_folder)

def go_combination():
	for dataset in scenarios:
		combination_rank1_neutral(dataset, moments, cuts, dump="../tutu/", n=3)
		combination_rank1_nonneutral(dataset, moments, cuts, dump="../tutu/", n=3)
		combination_roc1(dataset, moments, cuts, dump="../tutu/", n=3)

if __name__ == "__main__":
	extractor = MomentExtractor()
	extractor.totalExtraction(faces, scenarios, cuts, moments, "tutu")
	go_classification_rank1("tutu")
	#go_classification_roc1("tutu")
	
	#go_combination()
	
	"""
	frontal_neutral = "../tutu/frontal/bosphorus-outlier-densit200-crop80-icp/neutral-spheric.dat"
	sagittal_neutral = "../tutu/sagittal/bosphorus-outlier-densit200-crop80-icp/neutral-spheric.dat"
	transversal_neutral = "../tutu/transversal/bosphorus-outlier-densit200-crop80-icp/neutral-spheric.dat"
	
	frontal_nonneutral = "../tutu/frontal/bosphorus-outlier-densit200-crop80-icp/nonneutral-spheric.dat"
	sagittal_nonneutral = "../tutu/sagittal/bosphorus-outlier-densit200-crop80-icp/nonneutral-spheric.dat"
	transversal_nonneutral = "../tutu/transversal/bosphorus-outlier-densit200-crop80-icp/nonneutral-spheric.dat"
	
	# NEUTRAL
	
	ans = rank1_neutral_concat([frontal_neutral, sagittal_neutral, transversal_neutral])
	classifier, rate = max_rate(ans)
	rate = round(rate*100, 2)
	print("NEUTRAL - FST:\t\t{}\t{}".format(classifier, rate))
	
	ans = rank1_neutral_concat([frontal_neutral, sagittal_neutral])
	classifier, rate = max_rate(ans)
	rate = round(rate*100, 2)
	print("NEUTRAL - FS:\t\t{}\t{}".format(classifier, rate))
	
	ans = rank1_neutral_concat([frontal_neutral, transversal_neutral])
	classifier, rate = max_rate(ans)
	rate = round(rate*100, 2)
	print("NEUTRAL - FT:\t\t{}\t{}".format(classifier, rate))
	
	ans = rank1_neutral_concat([sagittal_neutral, transversal_neutral])
	classifier, rate = max_rate(ans)
	rate = round(rate*100, 2)
	print("NEUTRAL - ST:\t\t{}\t{}".format(classifier, rate))
	
	# NONNEUTRAL
	
	ans = rank1_nonneutral_concat([frontal_neutral, sagittal_neutral, transversal_neutral], [frontal_nonneutral, sagittal_nonneutral, transversal_nonneutral])
	classifier, rate = max_rate(ans)
	rate = round(rate*100, 2)
	print("NONNEUTRAL - FST:\t{}\t{}".format(classifier, rate))
	
	ans = rank1_nonneutral_concat([frontal_neutral, sagittal_neutral], [frontal_nonneutral, sagittal_nonneutral])
	classifier, rate = max_rate(ans)
	rate = round(rate*100, 2)
	print("NONNEUTRAL - FS:\t{}\t{}".format(classifier, rate))
	
	ans = rank1_nonneutral_concat([frontal_neutral, transversal_neutral], [frontal_nonneutral, transversal_nonneutral])
	classifier, rate = max_rate(ans)
	rate = round(rate*100, 2)
	print("NONNEUTRAL - FT:\t{}\t{}".format(classifier, rate))
	
	ans = rank1_nonneutral_concat([sagittal_neutral, transversal_neutral], [sagittal_nonneutral, transversal_nonneutral])
	classifier, rate = max_rate(ans)
	rate = round(rate*100, 2)
	print("NONNEUTRAL - ST:\t{}\t{}".format(classifier, rate))
	
	# ROC1
	
	ans = roc1_concat([frontal_neutral, sagittal_neutral, transversal_neutral], [frontal_nonneutral, sagittal_nonneutral, transversal_nonneutral])
	classifier, rate = max_rate(ans)
	rate = round(rate*100, 2)
	print("ROC1 - FST:\t\t{}\t{}".format(classifier, rate))
	
	ans = roc1_concat([frontal_neutral, sagittal_neutral], [frontal_nonneutral, sagittal_nonneutral])
	classifier, rate = max_rate(ans)
	rate = round(rate*100, 2)
	print("ROC1 - FS:\t\t{}\t{}".format(classifier, rate))
	
	ans = roc1_concat([frontal_neutral, transversal_neutral], [frontal_nonneutral, transversal_nonneutral])
	classifier, rate = max_rate(ans)
	rate = round(rate*100, 2)
	print("ROC1 - FT:\t\t{}\t{}".format(classifier, rate))
	
	ans = roc1_concat([sagittal_neutral, transversal_neutral], [sagittal_nonneutral, transversal_nonneutral])
	classifier, rate = max_rate(ans)
	rate = round(rate*100, 2)
	print("ROC1 - ST:\t\t{}\t{}".format(classifier, rate))
	"""
	
