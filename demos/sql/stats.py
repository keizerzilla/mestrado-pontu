import parse
from recpad import *
from mpl_toolkits import mplot3d
from sklearn.linear_model import Ridge
from sklearn.preprocessing import PolynomialFeatures
from sklearn.pipeline import make_pipeline
from sklearn.linear_model import LinearRegression

def analysis_distances():
	df = pd.read_csv("aly_dist.csv")
	print("analisando dados...")
	norms = {"sub" : [], "crop" : [], "norm" : []}
	for sub in df["subject"].unique():
		subject = df.loc[df["subject"] == sub]
		for crop in [20, 40, 60]:
			points = subject.loc[subject["crop"] == crop][["x","y","z"]]
			points = np.array(points)
			
			fsub = parse.parse("bs{}_N_N_{}", sub)[0]
			
			norms["sub"].append(fsub)
			norms["crop"].append(crop)
			norms["norm"].append(np.linalg.norm(np.cov(points.T), ord="fro"))

	n = pd.DataFrame.from_dict(norms)
	n.to_csv("frobenius.csv", index=False)

def analysis_stats():
	df = pd.read_csv("frobenius.csv")
	decimals = 2
	print("crop\tmedia\tmediana\tmin\tmax\tstd")
	for c in [20, 40, 60]:
		sample = df.loc[df["crop"] == c]
		sample = sample["norm"]
		
		mean = round(sample.mean(), decimals)
		median = round(sample.median(), decimals)
		minv = round(sample.min(), decimals)
		maxv = round(sample.max(), decimals)
		std = round(sample.std(), decimals)
		
		print("{}\t{}\t{}\t{}\t{}\t{}".format(c, mean, median, minv, maxv, std))

def analysis_plot():
	df = pd.read_csv("frobenius.csv")
	sb.pairplot(df, hue="crop", diag_kind="hist")
	plt.show()
	
def analysis_classification():
	classifiers = {"NN"  : NN(n_neighbors=1)}
	df = pd.read_csv("frobenius.csv")
	X = df["norm"]
	y = df["sub"]
	X = np.array(X).reshape(-1, 1)
	
	ans = classify(classifiers, X, y, 0.1, 100)
	sumary(ans)

def analysis_fit():
	df = pd.read_csv("aly_dist.csv")
	
	print("analisando dados...")
	for sub in df["subject"].unique():
		subject = df.loc[df["subject"] == sub]
		for crop in [20, 40, 60]:
			points = subject.loc[subject["crop"] == crop][["x","y","z"]]
			xy = points[["x", "y"]]
			z = points["z"]
			
			X = xy
			y = z
			reg = LinearRegression().fit(X, y)
			
			fig = plt.figure()
			ax = plt.axes(projection='3d')
			ax.scatter3D(X["x"], X["y"], y)
			plt.show()

if __name__ == "__main__":
	#analysis_distances()
	analysis_stats()
	analysis_plot()
	#analysis_fit()
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
