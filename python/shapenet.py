import os
import time
import subprocess
import numpy as np
import pandas as pd
from sklearn.svm import SVC as SVM
from sklearn.metrics import f1_score
from sklearn.metrics import recall_score
from sklearn.naive_bayes import GaussianNB
from sklearn.metrics import accuracy_score
from sklearn.metrics import precision_score
from sklearn.metrics import confusion_matrix
from sklearn.ensemble import AdaBoostClassifier
from sklearn.tree import DecisionTreeClassifier
from sklearn.preprocessing import StandardScaler
from sklearn.preprocessing import PowerTransformer
from sklearn.ensemble import RandomForestClassifier
from sklearn.neighbors import NearestCentroid as NC
from sklearn.neural_network import MLPClassifier as MLP
from sklearn.neighbors import KNeighborsClassifier as KNN
from sklearn.discriminant_analysis import LinearDiscriminantAnalysis as LDA
from sklearn.discriminant_analysis import QuadraticDiscriminantAnalysis as QDA
from sklearn.decomposition import PCA
from sklearn.model_selection import PredefinedSplit
from sklearn.model_selection import GridSearchCV
from joblib import dump
from joblib import load
from sklearn.feature_selection import VarianceThreshold

def extraction():
	dataset = "../datasets/SNCV2/"
	bindir = "../pontu/bin/shapenet"
	modelFile = "models/model_normalized.obj"
	moments = ["zi", "zr", "zm", "zf", "hs"]

	count = 0
	for moment in moments:
		dumpPath = "../results/shapenet_{}.dat".format(moment)
		with open(dumpPath, "w") as dump:
			for shape in os.listdir(dataset):
				objectPath = os.path.join(dataset, shape)
				
				if not os.path.isdir(objectPath):
					continue
				
				for sample in os.listdir(objectPath):
					samplePath = os.path.join(objectPath, sample)
					
					if not os.path.isdir(samplePath):
						continue
					
					samplePath = os.path.join(samplePath, modelFile)
					
					if not os.path.isfile(samplePath):
						continue
					
					cmd = [bindir, samplePath, moment]
					ans = subprocess.run(cmd, stdout=subprocess.PIPE).stdout
					ans = ans[:-1].decode("utf-8").replace(" ", ",")
					ans = ans + ",{},{}\n".format(sample, shape)
					dump.write(ans)
					
					count = count + 1
					print(moment, sample, "ok!", count)

def grid_search(datFile, splitFile):
	# LOADING DATA FILE
	df = pd.read_csv(datFile, header=None)
	cols = ["z{}".format(x) for x in range(len(df.columns)-2)]
	cols = cols + ["sample", "class"]
	df.columns = cols

	# LOADING TRAIN_TEST_VALIDATION SPLIT FILE
	split = pd.read_csv(splitFile)
	split = split.drop(["id", "synsetId", "subSynsetId"], axis=1)

	# SETTING SPLIT VARIABLES 1
	train = split.loc[split["split"] == "train"]
	test = split.loc[split["split"] == "test"]
	val = split.loc[split["split"] == "val"]

	# SETTING SPLIT VARIABLES 2
	train_set = df.loc[df["sample"].isin(train["modelId"])]
	test_set = df.loc[df["sample"].isin(test["modelId"])]
	val_set = df.loc[df["sample"].isin(val["modelId"])]

	# SETTING SPLIT VARIABLES 3
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

	# TRAIN + VAL
	X_trainval = np.concatenate([X_train, X_val])
	y_trainval = pd.concat([y_train, y_val])
	prefold = [-1 for x in range(X_train.shape[0])] + \
		      [0 for x in range(X_val.shape[0])]

	# GRIDSEARCHCV - KNN
	ps = PredefinedSplit(prefold)
	knn = KNN()

	param_grid = {
		"p" : [1, 2],
		"n_neighbors" : [5, 6, 7, 8, 9, 10],
		"weights" : ["uniform", "distance"]
	}

	grid = GridSearchCV(knn, param_grid=param_grid, n_jobs=-1, cv=ps)
	grid.fit(X_trainval, y_trainval)
	print(grid.best_estimator_)
	print(grid.best_score_)
	print(grid.best_params_)

	# GRIDSEARCHCV - SVM
	ps = PredefinedSplit(prefold)
	svm = SVM()

	param_grid = {
		"C" : [1.0, 2.0, 3.0, 4.0],
		"kernel" : ["rbf", "poly"],
		"gamma" : ["scale"],
	}

	grid = GridSearchCV(svm, param_grid=param_grid, n_jobs=-1, cv=ps)
	grid.fit(X_trainval, y_trainval)
	print(grid.best_estimator_)
	print(grid.best_score_)
	print(grid.best_params_)

	# GRIDSEARCHCV - RANDOM FOREST
	ps = PredefinedSplit(prefold)
	randforest = RandomForestClassifier()

	param_grid = {
		"n_estimators" : [500, 600],
		"min_samples_split" : [2],
		"min_samples_leaf" : [1],
		"max_features" : ["auto"]
	}

	grid = GridSearchCV(randforest, param_grid=param_grid, n_jobs=-1, cv=ps)
	grid.fit(X_trainval, y_trainval)
	print(grid.best_estimator_)
	print(grid.best_score_)
	print(grid.best_params_)

def single_results(datFiles, splitFile):
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
			accuracy = classifier.score(X_test, y_test)
			
			accuracy = round(100 * accuracy, 2)
			
			ans["classifier"].append(name)
			ans["accuracy"].append(accuracy)
			
			print("{}\t{}\t{}".format(datFile, name, accuracy))
		
		ans = pd.DataFrame(ans)
		ans.to_csv(datFile.replace(".dat", "_ans.csv"), index=None)
		
		del ans
		del classifiers

if __name__ == "__main__":
	datFiles = ["../results/shapenet/shapenet_sm.dat",
				"../results/shapenet/shapenet_zo.dat",
		        "../results/shapenet/shapenet_ze.dat",
		        "../results/shapenet/shapenet_zm.dat",
		        "../results/shapenet/shapenet_zf.dat",]
	splitFile = "../shapenet/all.csv"
	
	single_results(dataFiles, splitFile)
	
