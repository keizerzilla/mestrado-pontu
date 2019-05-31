import os
import pywt
import parse
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from sklearn.svm import SVC as SVM
from sklearn.naive_bayes import GaussianNB
from sklearn.ensemble import AdaBoostClassifier
from sklearn.tree import DecisionTreeClassifier
from sklearn.preprocessing import StandardScaler
from sklearn.preprocessing import PowerTransformer
from sklearn.ensemble import RandomForestClassifier
from sklearn.neighbors import NearestCentroid as NC
from sklearn.neighbors import KNeighborsClassifier as KNN

def cov_features(w):
	l1 = w[0]
	l2 = w[1]
	l3 = w[2]
	
	linearity = (l1 - l2) / l1
	planarity = (l2 - l3) / l1
	sphericity = l3 / l1
	omnivariance = pow(l1*l2*l3, 1.0/3.0)
	anisotropy = (l1 - l3) / l1
	eigenetropy = -1.0 * (l1*np.log(l1) + l2*np.log(l2) + l3*np.log(l3))
	suml = l1 + l2 + l3
	change_curv = l3 / (l1 + l2 + l3)
	
	ans = [linearity, planarity, sphericity, omnivariance, anisotropy,
	       eigenetropy, suml, change_curv]
	
	return ans

def cov_recog(folder, wavelet, results):
	
	fmt = "bs{:d}_{:w}_{:w}_{:d}.xyz"
	
	dump = {"linearity"    : [],
			"planarity"    : [],
			"sphericity"   : [],
			"omnivariance" : [],
			"anisotropy"   : [],
			"eigenetropy"  : [],
			"suml"         : [],
			"change_curv"  : [],
			"tp"           : [],
			"exp"          : [],
			"subject"      : [],
			"sample"       : []
	}
	
	for cloud in os.listdir(folder):
		if ".xyz" in cloud:
			input_file = folder + cloud
			match = parse.parse(fmt, cloud)
			
			subject = str(match[0])
			tp = str(match[1])
			exp = str(match[2])
			sample = str(match[3])
			
			data = np.genfromtxt(input_file, delimiter=" ")
			
			numrows = data.shape[0]
			if numrows % 2 != 0:
				data = np.concatenate([data, np.zeros((1, 3))])
			
			cx, dx = pywt.dwt(data[:, 0], wavelet)
			cy, dy = pywt.dwt(data[:, 1], wavelet)
			cz, dz = pywt.dwt(data[:, 2], wavelet)
			
			rec = np.array([cx, cy, cz])
			cov = np.cov(rec)
			w, v = np.linalg.eig(cov)
			w = w[w.argsort()[::-1]]
			
			feat = cov_features(w)
			
			dump["linearity"].append(feat[0])
			dump["planarity"].append(feat[1])
			dump["sphericity"].append(feat[2])
			dump["omnivariance"].append(feat[3])
			dump["anisotropy"].append(feat[4])
			dump["eigenetropy"].append(feat[5])
			dump["suml"].append(feat[6])
			dump["change_curv"].append(feat[7])
			dump["tp"].append(tp)
			dump["exp"].append(exp)
			dump["subject"].append(subject)
			dump["sample"].append(sample)
	
	dump = pd.DataFrame(dump)
	dump.to_csv(results, index=False)

def run_classification(X_train, y_train, X_test, y_test):
	classifiers = {
		"KNN_manhattam" : KNN(p=1, n_neighbors=1),
		"KNN_euclidean" : KNN(p=2, n_neighbors=1),
		"DMC_manhattam" : NC(metric="manhattan"),
		"DMC_euclidean" : NC(metric="euclidean"),
		"SVM_radial"    : SVM(kernel="rbf", gamma="auto"),
		"SVM_poly"      : SVM(kernel="poly", gamma="auto"),
		"GaussianNB"    : GaussianNB(),
		"RandomForest"  : RandomForestClassifier(n_estimators=100),
		"AdaBoost"      : AdaBoostClassifier(),
		"DecisionTree"  : DecisionTreeClassifier()
	}
	
	ans = dict()
	
	try:
		scaler = StandardScaler().fit(X_train)
		X_train = scaler.transform(X_train)
		X_test = scaler.transform(X_test)
	except:
		result = dict()
		result["recog"] = 0
		result["y_true"] = []
		result["y_pred"] = []
		print("! normalization error !")
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
		
		try:
			classifier.fit(X_train, y_train)
			score = classifier.score(X_test, y_test)
		except:
			print("! fit error !", name)
			score = 0
		
		result["recog"] = score
		result["y_true"] = y_test
		
		try:
			result["y_pred"] = classifier.predict(X_test)
		except:
			print("! predict error !", name)
			result["y_pred"] = []
		
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

def cov_analysis(datfile):
	df = pd.read_csv(datfile)
	feat = df.drop(["tp", "exp", "subject", "sample"], axis=1)
	print(feat.std())

if __name__ == "__main__":
	folder1 = "../datasets/bosphorus-outlier-densit200-crop80-icp/neutral/"
	folder2 = "../datasets/bosphorus/neutral/"
	results = "../datasets/bosphorus-wavelet/neutral/"
	datfile = "../datasets/bosphorus-wavelet/haar.dat"
	
	dump = {
		"wavelet"       : [],
		"KNN_manhattam" : [],
		"KNN_euclidean" : [],
		"DMC_manhattam" : [],
		"DMC_euclidean" : [],
		"SVM_radial"    : [],
		"SVM_poly"      : [],
		"GaussianNB"    : [],
		"RandomForest"  : [],
		"AdaBoost"      : [],
		"DecisionTree"  : []
	}
	
	for wavelet in pywt.wavelist(kind="discrete"):
		w_name = wavelet.replace(".", "-")
		datfile = "../datasets/bosphorus-wavelet/{}.dat".format(w_name)
		
		print("running wavelet", wavelet)
		cov_recog(folder2, wavelet, datfile)
		print(wavelet, "done!")
		
		dump["wavelet"].append(wavelet)
		
		ans = rank_neutral(datfile)
		for name, v in ans.items():
			score = round(100*v["recog"], 2)
			dump[name].append(score)
			print(name, score)
	
	dump = pd.DataFrame(dump)
	dump.to_csv("wavelet-results.csv", index=False)
	
	
