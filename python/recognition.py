import os
import sys
import time
import parse
import seaborn
import operator
import warnings
import subprocess
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from open3d import *
from sklearn.svm import SVC as SVM
from sklearn.decomposition import PCA
from sklearn.naive_bayes import GaussianNB
from sklearn.metrics import confusion_matrix
from sklearn.ensemble import AdaBoostClassifier
from sklearn.tree import DecisionTreeClassifier
from sklearn.preprocessing import StandardScaler
from sklearn.preprocessing import PowerTransformer
from sklearn.ensemble import RandomForestClassifier
from sklearn.neighbors import NearestCentroid as NC
from sklearn.neighbors import KNeighborsClassifier as KNN
from sklearn.model_selection import train_test_split as data_split
from sklearn.discriminant_analysis import LinearDiscriminantAnalysis as LDA
from sklearn.discriminant_analysis import QuadraticDiscriminantAnalysis as QDA

warnings.filterwarnings("ignore")

classifiers = {
	#"KNN_manhattam" : KNN(p=1, n_neighbors=1),
	#"KNN_euclidean" : KNN(p=2, n_neighbors=1),
	#"DMC_manhattam" : NC(metric="manhattan"),
	#"DMC_euclidean" : NC(metric="euclidean"),
	"SVM_radial"    : SVM(kernel="rbf", C=8.0, gamma=0.125),
	"SVM_poly"      : SVM(kernel="poly", C=8.0, gamma=0.125),
	#"LDA"           : LDA(),
	#"QDA"           : QDA(),
	#"GaussianNB"    : GaussianNB(),
	#"RandomForest"  : RandomForestClassifier(n_estimators=100),
	#"AdaBoost"      : AdaBoostClassifier(),
	#"DecisionTree"  : DecisionTreeClassifier()
}

def registration(input_folder, nose_folder, output_folder):
	#------------------------#
	# |                    | #
	# | UNDER CONSTRUCTION | #
	# |                    | #
	#------------------------#
	for bsdir in os.listdir(input_folder):
		folder = input_folder + bsdir
		nose_path = nose_folder + bsdir
		result_folder = output_folder + bsdir
		if os.path.isdir(folder):
			for cloud in os.listdir(folder):
				if cloud.endswith(".pcd"):
					fullPath = folder + "/" + cloud
					fullNose = nose_path + "/" + cloud
					
					print(result_folder)

def cloud_extraction(cloud, nose, cut):
	mcalc = "../bin/siqcalc"
	fmt = "bs{:d}_{:w}_{:w}_{:d}.pcd"
	path, filename = os.path.split(cloud)
	match = parse.parse(fmt, filename)
	
	if match == None:
		return None
	
	subject = str(match[0])
	tp = str(match[1])
	ex = str(match[2])
	sample = str(match[3])
	
	cmd = [mcalc, "-i", cloud, "-n", nose, "-o", "stdout", "-c", cut]
	ans = subprocess.run(cmd, stdout=subprocess.PIPE).stdout
	ans = ans[:-1].decode("utf-8").replace(" ", ",")
	ans = ans + ",{},{},{},{}\n".format(sample, subject, tp, ex)
	
	return ans

def batch_extraction(dataset, noses, cut, output):
	print("[  siqueira  ] - [ {} ] - [{}]".format(cut, dataset))
	
	count = 0
	start_time = time.time()
	
	
	dump = []
	for bsdir in os.listdir(dataset):
		folder = dataset + bsdir
		noseFolder = noses + bsdir
		if os.path.isdir(folder):
			for cloud in os.listdir(folder):
				if cloud.endswith(".pcd"):
					fullPath = folder + "/" + cloud
					fullNose = noseFolder + "/" + cloud
					
					ans = cloud_extraction(fullPath, fullNose, cut)
					if ans == None:
						continue
					
					dump.append(ans)
					count = count + 1
					print("\r{}".format(count), end="\r")
	
	elapsed = time.time() - start_time
	vel = round(elapsed / count, 6)
	
	cols = ["m" + str(x) for x in range(len(dump[0].split(",")) - 4)]
	cols = cols + ["sample", "subject", "tp", "exp"]
	cols = ",".join(cols) + "\n"
	dump = cols + "".join(dump)
	
	data_file = open(output, "w")
	data_file.write(dump)
	data_file.close()
	
	print("[     OK     ] - [ {} nuvens, {} seg/nuvem]".format(count, vel))

def run_classification(X_train, y_train, X_test, y_test):
	ans = dict()
	
	X_train[X_train == np.inf] = 0
	X_train[X_train == -np.inf] = 0
	X_train[X_train == np.nan] = 0
	X_test[X_test == np.inf] = 0
	X_test[X_test == -np.inf] = 0
	X_test[X_test == np.nan] = 0
	
	try:
		scaler = StandardScaler().fit(X_train)
		X_train = scaler.transform(X_train)
		X_test = scaler.transform(X_test)
	except:
		result = dict()
		result["recog"] = 0
		result["y_true"] = []
		result["y_pred"] = []
		ans["ERROR_NORM"] = result
		return ans
	
	new_train = []
	new_test = []
	
	try:
		pt = PowerTransformer(method="yeo-johnson", standardize=False)
		for m_train, m_test in zip(X_train.T, X_test.T):
			data_train = m_train.reshape(-1, 1)
			data_test = m_test.reshape(-1, 1)
			pt.fit(data_train)
			new_train.append(np.ravel(pt.transform(data_train)))
			new_test.append(np.ravel(pt.transform(data_test)))
	except:
		result = dict()
		result["recog"] = 0
		result["y_true"] = []
		result["y_pred"] = []
		ans["ERROR_SKEW"] = result
		return ans
	
	X_train = np.array(new_train).T
	X_test = np.array(new_test).T
	
	for name, classifier in classifiers.items():
		result = dict()
		start_time = time.time()
		
		try:
			classifier.fit(X_train, y_train)
			score = classifier.score(X_test, y_test)
		except:
			score = 0
		
		result["recog"] = score
		result["y_true"] = y_test
		
		try:
			result["y_pred"] = classifier.predict(X_test)
		except:
			result["y_pred"] = []
		
		elapsed_time = str(round(time.time() - start_time, 4))
		result["time"] = elapsed_time
		ans[name] = result
	
	return ans
	
def rank_neutral(features):
	df = pd.read_csv(features)
	
	cond_train = (df["tp"] == "N") & (df["sample"] == 0)
	trainset = df.loc[cond_train].drop(["sample", "tp", "exp"], axis=1)
	X_train = np.array(trainset.drop(["subject"], axis=1))
	y_train = np.ravel(trainset[["subject"]])
	
	cond_test = (df["tp"] == "N") & (df["sample"] != 0)
	testset = df.loc[cond_test].drop(["sample", "tp", "exp"], axis=1)
	X_test = np.array(testset.drop(["subject"], axis=1))
	y_test = np.ravel(testset[["subject"]])
	
	return run_classification(X_train, y_train, X_test, y_test)

def rank_nonneutral(features):
	df = pd.read_csv(features)
	
	cond_train = (df["tp"] == "N") & (df["sample"] == 0)
	trainset = df.loc[cond_train].drop(["sample", "tp", "exp"], axis=1)
	X_train = np.array(trainset.drop(["subject"], axis=1))
	y_train = np.ravel(trainset[["subject"]])
	
	cond_test = (df["tp"] == "E") | \
	            (df["tp"] == "UFAU") | \
	            (df["tp"] == "LFAU") | \
	            (df["tp"] == "CAU")
	testset = df.loc[cond_test].drop(["sample", "tp", "exp"], axis=1)
	X_test = np.array(testset.drop(["subject"], axis=1))
	y_test = np.ravel(testset[["subject"]])
	
	return run_classification(X_train, y_train, X_test, y_test)

def roc3(features):
	df = pd.read_csv(features)
	
	cond_train = (df["tp"] == "N")
	trainset = df.loc[cond_train].drop(["sample", "tp", "exp"], axis=1)
	X_train = np.array(trainset.drop(["subject"], axis=1))
	y_train = np.ravel(trainset[["subject"]])
	
	cond_test = (df["tp"] == "E") | \
	            (df["tp"] == "UFAU") | \
	            (df["tp"] == "LFAU") | \
	            (df["tp"] == "CAU")
	testset = df.loc[cond_test].drop(["sample", "tp", "exp"], axis=1)
	X_test = np.array(testset.drop(["subject"], axis=1))
	y_test = np.ravel(testset[["subject"]])
	
	return run_classification(X_train, y_train, X_test, y_test)

def rank_occlusion(features):
	df = pd.read_csv(features)
	
	cond_train = (df["tp"] == "N") & (df["sample"] == 0)
	trainset = df.loc[cond_train].drop(["sample", "tp", "exp"], axis=1)
	X_train = np.array(trainset.drop(["subject"], axis=1))
	y_train = np.ravel(trainset[["subject"]])
	
	cond_test = (df["tp"] == "O")
	testset = df.loc[cond_test].drop(["sample", "tp", "exp"], axis=1)
	X_test = np.array(testset.drop(["subject"], axis=1))
	y_test = np.ravel(testset[["subject"]])
	
	return run_classification(X_train, y_train, X_test, y_test)

def max_recognition(ans, msg):
	rates = dict((key, value["recog"]) for key, value in ans.items())
	lemax = max(rates.items(), key=operator.itemgetter(1))
	
	classifier, rate = lemax[0], round(lemax[1]*100, 2)
	print("{:<15}{:<15}{:<7}".format(msg, classifier, rate))
	
	return classifier, rate

def confusion(y_true, y_pred):
	classes = [x for x in range(105)]
	
	seaborn.set()
	
	conf = confusion_matrix(y_true, y_pred, labels=classes)
	seaborn.heatmap(conf, cmap="Purples")
	
	tick_marks = np.arange(len(classes))
	plt.xticks(tick_marks, classes, rotation=90)
	plt.yticks(tick_marks, classes, rotation=360)
	
	plt.title("Matriz de Confusão")
	plt.ylabel("Verdade terrestre")
	plt.xlabel("Classes estimadas")
	plt.tight_layout()
	plt.show()
	
