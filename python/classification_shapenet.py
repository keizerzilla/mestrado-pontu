import pandas as pd
from sklearn.svm import SVC as SVM
from sklearn.decomposition import PCA
from sklearn.ensemble import VotingClassifier
from sklearn.preprocessing import StandardScaler
from sklearn.preprocessing import PowerTransformer
from sklearn.feature_selection import VarianceThreshold
from sklearn.neighbors import KNeighborsClassifier as KNN
from sklearn.ensemble import RandomForestClassifier as RandomForest

def single_committe():
	# FILES
	datFiles = ["../results/shapenet/shapenet_zo.dat",
		        #"../results/shapenet/shapenet_ze.dat",
		        #"../results/shapenet/shapenet_zm.dat",
		        #"../results/shapenet/shapenet_zf.dat",
				"../results/shapenet/shapenet_so.dat",]
		        #"../results/shapenet/shapenet_se.dat",
		        #"../results/shapenet/shapenet_sm.dat",
		        #"../results/shapenet/shapenet_sf.dat",]
	splitFile = "../shapenet/all.csv"

	# LOADING TRAIN_TEST_VALIDATION SPLIT FILE
	split = pd.read_csv(splitFile)
	split = split.drop(["id", "synsetId", "subSynsetId"], axis=1)

	# SETTING SPLIT VARIABLES
	train = split.loc[split["split"] == "train"]
	test = split.loc[split["split"] == "test"]
	val = split.loc[split["split"] == "val"]

	for datFile in datFiles:
		# LOADING DATA FILE
		df = pd.read_csv(datFile, header=None)
		n_features = len(df.columns) - 2
		
		feats = ["z{}".format(x) for x in range(n_features)]
		cols = feats + ["sample", "class"]
		df.columns = cols
		
		# SPLITIN SETS
		train_set = df.loc[df["sample"].isin(train["modelId"])]
		test_set = df.loc[df["sample"].isin(test["modelId"])]
		val_set = df.loc[df["sample"].isin(val["modelId"])]
		
		X_train = train_set.drop(["sample", "class"], axis=1)
		y_train = train_set["class"]
		X_test = test_set.drop(["sample", "class"], axis=1)
		y_test = test_set["class"]
		X_val = val_set.drop(["sample", "class"], axis=1)
		y_val = val_set["class"]
		
		# STANDARDIZATION
		scaler = StandardScaler()
		scaler.fit(X_train)
		X_train = scaler.transform(X_train)
		X_test = scaler.transform(X_test)
		X_val = scaler.transform(X_val)
		
		# SKEW REMOVAL
		pt = PowerTransformer(method="yeo-johnson", standardize=False)
		pt.fit(X_train)
		X_train = pt.transform(X_train)
		X_test = pt.transform(X_test)
		X_val = pt.transform(X_val)
		
		# CLASSIFIERS
		knn = KNN(n_neighbors=8, weights="distance")
		svm = SVM(C=3, gamma="scale", kernel="rbf")
		rfc = RandomForest(n_estimators=500)
		estimators = [("knn", knn), ("svm", svm), ("rfc", rfc)]
		
		classifiers = {
			"hard" : VotingClassifier(estimators=estimators, voting="hard"),
			#"soft" : VotingClassifier(estimators=estimators, voting="soft"),
		}
		
		ans = {
			"classifier" : [],
			"accuracy"   : [],
		}
		
		for name, classifier in classifiers.items():
			# CLASSIFICATION
			classifier.fit(X_train, y_train)
			accuracy = classifier.score(X_val, y_val)
			
			accuracy = round(100 * accuracy, 2)
			
			ans["classifier"].append(name)
			ans["accuracy"].append(accuracy)
			
			print("{}\t{}\t{}".format(datFile, name, accuracy))

def full_results():
	# DATA FILES
	dataFile = "../results/shapenet_zernike_full.dat"
	splitFile = "../shapenet/all.csv"
	
	# LOADING TRAIN_TEST_VALIDATION SPLIT FILE
	split = pd.read_csv(splitFile)
	split = split.drop(["id", "synsetId", "subSynsetId"], axis=1)
	
	# SETTING SPLIT VARIABLES
	train = split.loc[split["split"] == "train"]
	test = split.loc[split["split"] == "test"]
	val = split.loc[split["split"] == "val"]
	
	# LOADING DATA
	df = pd.read_csv(dataFile)
	
	# SPLITIN SETS
	train_set = df.loc[df["sample"].isin(train["modelId"])]
	test_set = df.loc[df["sample"].isin(test["modelId"])]
	val_set = df.loc[df["sample"].isin(val["modelId"])]
	
	X_train = train_set.drop(["sample", "class"], axis=1)
	y_train = train_set["class"]
	X_test = test_set.drop(["sample", "class"], axis=1)
	y_test = test_set["class"]
	X_val = val_set.drop(["sample", "class"], axis=1)
	y_val = val_set["class"]
	
	# STANDARDIZATION
	scaler = StandardScaler()
	scaler.fit(X_train)
	X_train = scaler.transform(X_train)
	X_test = scaler.transform(X_test)
	X_val = scaler.transform(X_val)
	
	# SKEW REMOVAL
	pt = PowerTransformer(method="yeo-johnson", standardize=False)
	pt.fit(X_train)
	X_train = pt.transform(X_train)
	X_test = pt.transform(X_test)
	X_val = pt.transform(X_val)
	
	# CLASSIFIERS
	classifiers = {
		"kNN" : KNN(n_neighbors=8, weights="distance"),
		"SVM" : SVM(C=3, gamma="scale", kernel="rbf"),
		"RFC" : RandomForest(n_estimators=500),
	}
	
	ans = {
		"classifier" : [],
		"accuracy"   : [],
	}
	
	print("SOCORRO")
	for name, classifier in classifiers.items():
		# CLASSIFICATION
		classifier.fit(X_train, y_train)
		accuracy = classifier.score(X_val, y_val)
	
		accuracy = round(100 * accuracy, 2)
		
		ans["classifier"].append(name)
		ans["accuracy"].append(accuracy)
		
		print("{}\t{}\t{}".format("FULL", name, accuracy))

def joining_results():
	# DATA FILES
	oddFile = "../results/shapenet_zernike_odd.dat"
	evenFile = "../results/shapenet_zernike_even.dat"
	magFile = "../results/shapenet_zernike_mag.dat"
	splitFile = "../shapenet/all.csv"

	# LOADING TRAIN_TEST_VALIDATION SPLIT FILE
	split = pd.read_csv(splitFile)
	split = split.drop(["id", "synsetId", "subSynsetId"], axis=1)

	# SETTING SPLIT VARIABLES
	train = split.loc[split["split"] == "train"]
	test = split.loc[split["split"] == "test"]
	val = split.loc[split["split"] == "val"]
	
	# LOADING DATA FILES
	odd = pd.read_csv(oddFile)
	even = pd.read_csv(evenFile)
	mag = pd.read_csv(magFile)
	
	oddFeatures = len(odd.columns) - 2
	oddFeatlist = ["zodd{}".format(x) for x in range(oddFeatures)]
	oddCols = oddFeatlist + ["sample", "class"]
	odd.columns = oddCols
	
	evenFeatures = len(even.columns) - 2
	evenFeatlist = ["zeven{}".format(x) for x in range(evenFeatures)]
	evenCols = evenFeatlist + ["sample", "class"]
	even.columns = evenCols
	
	magFeatures = len(mag.columns) - 2
	magFeatlist = ["zmag{}".format(x) for x in range(magFeatures)]
	magCols = magFeatlist + ["sample", "class"]
	mag.columns = magCols
	
	odd = odd.set_index(["sample", "class"])
	even = even.set_index(["sample", "class"])
	mag = mag.set_index(["sample", "class"])
	
	# JOINING:
	df = odd.join(even)
	df = df.join(mag)
	df.to_csv("../results/shapenet_zernike_full.dat")
	
	
def single_results():
	# FILES
	datFiles = ["../results/shapenet/shapenet_zo.dat",
		        #"../results/shapenet/shapenet_ze.dat",
		        #"../results/shapenet/shapenet_zm.dat",
		        #"../results/shapenet/shapenet_zf.dat",
				"../results/shapenet/shapenet_so.dat",]
		        #"../results/shapenet/shapenet_se.dat",
		        #"../results/shapenet/shapenet_sm.dat",
		        #"../results/shapenet/shapenet_sf.dat",]
	splitFile = "../shapenet/all.csv"

	# LOADING TRAIN_TEST_VALIDATION SPLIT FILE
	split = pd.read_csv(splitFile)
	split = split.drop(["id", "synsetId", "subSynsetId"], axis=1)

	# SETTING SPLIT VARIABLES
	train = split.loc[split["split"] == "train"]
	test = split.loc[split["split"] == "test"]
	val = split.loc[split["split"] == "val"]

	for datFile in datFiles:
		# LOADING DATA FILE
		df = pd.read_csv(datFile, header=None)
		n_features = len(df.columns) - 2
		
		feats = ["z{}".format(x) for x in range(n_features)]
		cols = feats + ["sample", "class"]
		df.columns = cols
		
		# SPLITIN SETS
		train_set = df.loc[df["sample"].isin(train["modelId"])]
		test_set = df.loc[df["sample"].isin(test["modelId"])]
		val_set = df.loc[df["sample"].isin(val["modelId"])]
		
		X_train = train_set.drop(["sample", "class"], axis=1)
		y_train = train_set["class"]
		X_test = test_set.drop(["sample", "class"], axis=1)
		y_test = test_set["class"]
		X_val = val_set.drop(["sample", "class"], axis=1)
		y_val = val_set["class"]
		
		# REMOVE ZERO VARIANCE
		selector = VarianceThreshold()
		X_train = selector.fit_transform(X_train)
		X_test = selector.fit_transform(X_test)
		X_val = selector.fit_transform(X_val)
		
		# STANDARDIZATION
		scaler = StandardScaler()
		scaler.fit(X_train)
		X_train = scaler.transform(X_train)
		X_test = scaler.transform(X_test)
		X_val = scaler.transform(X_val)
		
		# SKEW REMOVAL
		pt = PowerTransformer(method="yeo-johnson", standardize=False)
		pt.fit(X_train)
		X_train = pt.transform(X_train)
		X_test = pt.transform(X_test)
		X_val = pt.transform(X_val)
		
		# CLASSIFIERS
		classifiers = {
			"kNN" : KNN(n_neighbors=8, weights="distance"),
			"SVM" : SVM(C=3, gamma="scale", kernel="rbf"),
			"RFC" : RandomForest(n_estimators=500),
		}
		
		ans = {
			"classifier" : [],
			"accuracy"   : [],
		}
		
		for name, classifier in classifiers.items():
			# CLASSIFICATION
			classifier.fit(X_train, y_train)
			accuracy = classifier.score(X_val, y_val)
			
			accuracy = round(100 * accuracy, 2)
			
			ans["classifier"].append(name)
			ans["accuracy"].append(accuracy)
			
			print("{}\t{}\t{}".format(datFile, name, accuracy))

if __name__ == "__main__":
	single_results()
	
	
	
	
