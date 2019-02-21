from recpad import *
from dataset import *
from features import *
from classification import *

faces = ["neutral", "nonneutral"]

cuts = {"w" : "whole",
        "f" : "frontal",
        "s" : "sagittal",
        "t" : "transversal",
        "r" : "radial",
        "u" : "upper",
        "l" : "lower"}

replace_dict = {"bosphorus" : "bs",
                "outlier" : "out",
                "densit" : "d",
                "crop" : "c"}

scenarios= ["bosphorus",
            "bosphorus-outlier",
            "bosphorus-outlier-densit200",
            "bosphorus-outlier-densit225",
            "bosphorus-outlier-densit200-crop60",
			"bosphorus-outlier-densit200-crop70",
			"bosphorus-outlier-densit200-crop80",
			"bosphorus-outlier-densit225-crop60",
			"bosphorus-outlier-densit225-crop70",
			"bosphorus-outlier-densit225-crop80",
            "bosphorus-outlier-densit200-crop60-icp",
			"bosphorus-outlier-densit200-crop70-icp",
			"bosphorus-outlier-densit200-crop80-icp",
			"bosphorus-outlier-densit225-crop60-icp",
			"bosphorus-outlier-densit225-crop70-icp",
			"bosphorus-outlier-densit225-crop80-icp"]

mini_scenarios = []
for s in scenarios:
	temp = s
	for old, new in replace_dict.items():
		temp = temp.replace(old, new)
	mini_scenarios.append(temp)

moments = ["spheric",
           "hututu",
           "hu1980",
           "husiq",
           "zernike",
           "legendre",
           "chebyshev"]

def go_analysis(rdir):
	for face in faces:
		datasets = ["../datasets/" + x + "/{}".format(face) for x in scenarios]
		for cut, cut_folder in cuts.items():
			for moment in moments:
				for data in datasets:
					scenario = data.split("/")[2]
					folder = "../{}/{}/{}/".format(rdir, cut_folder, scenario)
					out = folder
					out = out + "{}-{}.dat".format(face, moment)
					df = pd.read_csv(out, header=None)
					print(out)

def go_extraction(rdir):
	for face in faces:
		datasets = ["../datasets/" + x + "/{}".format(face) for x in scenarios]
		for cut, cut_folder in cuts.items():
			for data in datasets:
				scenario = data.split("/")[2]
				folder = "../{}/{}/{}/".format(rdir, cut_folder, scenario)
				os.makedirs(folder, exist_ok=True)
				for moment in moments:
					out = folder
					out = out + "{}-{}.dat".format(face, moment)
					moment_extraction_batch(moment, cut, data, out)

def go_classification_rank1(rdir, table=False):
	for face in faces:
		datasets = ["../datasets/" + x + "/{}".format(face) for x in scenarios]
		for cut, cut_folder in cuts.items():
			
			res = pd.DataFrame(columns=moments, index=mini_scenarios)
			test_name = ""
			
			for data in datasets:
				scenario = data.split("/")[2]
				folder = "../{}/{}/{}/".format(rdir, cut_folder, scenario)
				#os.makedirs(folder, exist_ok=True)
				
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
			
			if table:
				ax = res.plot(marker='o', figsize=(11, 9))
				tick_marks_x = np.arange(len(mini_scenarios))
				tick_marks_y = range(0, 110, 10)
				plt.xticks(tick_marks_x, mini_scenarios, rotation=90)
				plt.yticks(tick_marks_y)
				filename = "{} - {}".format(test_name, cut_folder)
				plt.title(filename)
				plt.ylabel("Acerto (%)")
				plt.grid()
				
				for moment in moments:
					for x, y in enumerate(res[moment]):
						ax.annotate(str(y), xy=(x, y), xytext=(x-0.2, y+1))
				
				plt.tight_layout()
				plt.savefig("../" + rdir + "/figures/" + filename + ".png")

def go_classification_roc1(rdir, table=False):
	for face in faces:
		datasets = ["../datasets/" + x + "/{}".format(face) for x in scenarios]
		for cut, cut_folder in cuts.items():
			
			res = pd.DataFrame(columns=moments, index=mini_scenarios)
			
			for data in datasets:
				scenario = data.split("/")[2]
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
		
		if table:
			ax = res.plot(marker='o', figsize=(11, 9))
			tick_marks_x = np.arange(len(mini_scenarios))
			tick_marks_y = range(0, 110, 10)
			plt.xticks(tick_marks_x, mini_scenarios, rotation=90)
			plt.yticks(tick_marks_y)
			filename = "{} - {}".format(test_name, cut_folder)
			plt.title(filename)
			plt.ylabel("Acerto (%)")
			plt.grid()
			
			for moment in moments:
				for x, y in enumerate(res[moment]):
					ax.annotate(str(y), xy=(x, y), xytext=(x-0.2, y+1))
				
			plt.tight_layout()
			plt.savefig("../" + rdir + "/figures/" + filename + ".png")

def go_combination():
	for dataset in scenarios:
		combination_rank1_neutral(dataset, moments)

if __name__ == "__main__":
	go_classification_roc1(rdir="results", table=True)
	
	
	
	
	
