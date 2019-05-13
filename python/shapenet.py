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

def extraction(folder, moment, outfile):
	count = 0
	print("shapenet:", moment)
	with open(outfile, "w") as dump:
		for synsetid in os.listdir(folder):
			path = folder + synsetid + "/"
			if os.path.isdir(path):
				for modelid in os.listdir(path):
					model_path = path + modelid + "/models/model_normalized.obj"
					if os.path.isfile(model_path):
						cmd = ["../bin/shapenet", moment, model_path]
						
						ans = subprocess.run(cmd, stdout=subprocess.PIPE).stdout
						ans = ans[:-1].decode("utf-8").replace(" ", ",")
						ans = ans + ",{},{}\n".format(synsetid, modelid)
						
						dump.write(ans)
						count = count + 1
						print("\r{}".format(count), end="\r")

def classification(moment, datafile):
	print(moment)
	
	ans = {
		"classifier" : [],
		"accuracy"   : [],
		"precision"  : [],
		"recall"     : [],
		"f1"         : [],
		"elapsed"    : []
	}
	
	split = pd.read_csv("/home/artur/SHREC17/all.csv")
	train = split.loc[split["split"] == "train"]
	test = split.loc[split["split"] == "test"]
	val = split.loc[split["split"] == "val"]
	
	classifiers = {
		"KNN_manhattam" : KNN(p=1),
		"KNN_euclidean" : KNN(p=2),
		"SVM_radial"    : SVM(kernel="rbf", gamma="auto"),
		"SVM_linear"      : SVM(kernel="linear", gamma="auto"),
		"RandomForest"  : RandomForestClassifier(n_estimators=200),
		"MLP" : MLP(hidden_layer_sizes=(1000, ), max_iter=1000)
	}
	
	df = pd.read_csv(datafile, header=None)
	cols = ["z{}".format(m) for m in range(df.shape[1]-2)]
	cols = cols + ["synsetId", "modelId"]
	df.columns = cols
	
	train_set = df.loc[df["modelId"].isin(train["modelId"])]
	test_set = df.loc[df["modelId"].isin(test["modelId"])]
	
	X_train = train_set.drop(["synsetId", "modelId"], axis=1)
	y_train = train_set["synsetId"]
	X_test = test_set.drop(["synsetId", "modelId"], axis=1)
	y_test = test_set["synsetId"]
	
	scaler = StandardScaler()
	scaler.fit(X_train)
	X_train = scaler.transform(X_train)
	X_test = scaler.transform(X_test)
	
	pt = PowerTransformer(method="yeo-johnson", standardize=False)
	pt.fit(X_train)
	X_train = pt.transform(X_train)
	X_test = pt.transform(X_test)
	
	print("CLASSIFIER\tACC\tPRE\tREC\tF1\tTIME")
	for name, classifier in classifiers.items():
		start = time.time()
		
		classifier.fit(X_train, y_train)
		pred = classifier.predict(X_test)
		
		accuracy = round(accuracy_score(y_test, pred), 2)
		precision = round(precision_score(y_test, pred, average="micro"), 2)
		recall = round(recall_score(y_test, pred, average="micro"), 2)
		f1 = round(f1_score(y_test, pred, average="micro"), 2)
		
		elapsed = round(time.time() - start, 4)
		
		ans["classifier"].append(name)
		ans["accuracy"].append(accuracy)
		ans["precision"].append(precision)
		ans["recall"].append(recall)
		ans["f1"].append(f1)
		ans["elapsed"].append(elapsed)
		
		print("{}\t{}\t{}\t{}\t{}\t{}".format(name,
		                                      accuracy,
		                                      precision,
		                                      recall,
		                                      f1,
		                                      elapsed))
	
	ans = pd.DataFrame(ans)
	ans.to_csv("../shapenet/ans.csv", index=None)
	
if __name__ == "__main__":
	moments = ["spheric"]
	folder = "/home/artur/ShapeNetCore_v2/"
	
	for moment in moments:
		outfile = "../shapenet/" + moment + ".dat"
		classification(moment, outfile)
		
