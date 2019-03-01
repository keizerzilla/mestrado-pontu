import seaborn
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

faces = ["neutral",
         "nonneutral"]

cuts = {"w" : "whole",
        "f" : "frontal",
        "s" : "sagittal",
        "t" : "transversal",
        "r" : "radial",
        "u" : "upper",
        "l" : "lower"}

replace_dict = {"bosphorus" : "bs",
                "outlier"   : "out",
                "densit"    : "d",
                "crop"      : "c"}

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

moments = ["spheric",
           "hututu",
           "hu1980",
           "husiq",
           "zernike",
           "legendre",
           "chebyshev"]

def find_errors():
	ans = {"error"    : [],
           "cut"      : [],
           "scenario" : [],
           "face"     : [],
           "moment"   : [],
           "sample"   : []}
	
	for face in faces:
		datasets = ["../datasets/" + x + "/{}".format(face) for x in scenarios]
		for cut, cut_folder in cuts.items():
			for data in datasets:
				scenario = data.split("/")[2]
				folder = "../results/{}/{}/".format(cut_folder, scenario)
				for moment in moments:
					finput = folder + "{}-{}.dat".format(face, moment)
					
					df = pd.read_csv(finput, header=None)
					cols = ["f"+str(x) for x in range(len(df.columns)-4)]
					cols = cols + ["sample", "subject", "tp", "exp"]
					df.columns = cols
					
					if df.isnull().values.any():
						print("NaN found at file : " + finput)
						found = df[df.isnull().any(axis=1)]
						for index, row in found.iterrows():
							sample = "bs{:03d}_{}_{}_{}".format(row["subject"],
								                                row["tp"],
								                                row["exp"],
								                                row["sample"])
							ans["error"].append("notanumber")
							ans["cut"].append(cut_folder)
							ans["scenario"].append(scenario)
							ans["face"].append(face)
							ans["moment"].append(moment)
							ans["sample"].append(sample)
					if df.isin([np.inf, -np.inf]).values.any():
						print("inf found at file : " + finput)
						found = df[df.isin([np.inf, -np.inf]).any(axis=1)]
						for index, row in found.iterrows():
							sample = "bs{:03d}_{}_{}_{}".format(row["subject"],
								                                row["tp"],
								                                row["exp"],
								                                row["sample"])
							ans["error"].append("infinity")
							ans["cut"].append(cut_folder)
							ans["scenario"].append(scenario)
							ans["face"].append(face)
							ans["moment"].append(moment)
							ans["sample"].append(sample)
	
	results = pd.DataFrame(ans)
	results.to_csv("../results/naninf.csv", index=False)

def plot_errors():
	df = pd.read_csv("../results/naninf.csv")

	ax = seaborn.countplot(x="moment", hue="error", data=df)
	plt.title("Por tipo de erro")
	plt.savefig("../results/naninf_tipo.png")
	plt.close()

	ax = seaborn.countplot(x="moment", hue="cut", data=df)
	plt.title("Por tipo de segmentacao")
	plt.savefig("../results/naninf_cut.png")
	plt.close()

	ax = seaborn.countplot(x="moment", hue="face", data=df)
	plt.title("Por tipo de face")
	plt.savefig("../results/naninf_face.png")
	plt.close()

