import numpy as np
import pandas as pd
from sklearn.svm import SVC
from sklearn.neighbors import KNeighborsClassifier

"""
classification.py
Artur Rodrigues Rocha Neto
2018

Funções para classificação, análise de resultados e pré-processamento.
"""

"""
Executa classificação hardmode usando os 4 classificadores em pesquisa.

name -- O nome do teste em execução
features -- O caminho para o dataset de features
normalization -- Flag para indicar se a função deve normalizar os dados
"""
def classification(name, features, normalization):
	classifiers = [
		KNeighborsClassifier(p=1, n_neighbors=1),
		KNeighborsClassifier(p=2, n_neighbors=1),
		SVC(kernel="rbf"),
		SVC(kernel="poly")]

	names = [
		"KNN_manhattam",
		"KNN_euclidean",
		"SVM_radial",
		"SVM_poly"]
	
	df = pd.read_csv(features)
	
	trainset = df.loc[df["sample"] == 0].drop(["sample"], axis=1)
	X_train = np.array(trainset.drop(["subject"], axis=1))
	y_train = np.ravel(trainset[["subject"]])
	
	testset = df.loc[df["sample"] != 0].drop(["sample"], axis=1)
	X_test = np.array(testset.drop(["subject"], axis=1))
	y_test = np.ravel(testset[["subject"]])
	
	# normalizacao
	"""
	train_mean = np.mean(X_train)
	train_std = np.std(X_train)
	test_mean = np.mean(X_test)
	test_std = np.std(X_test)
	X_train = (X_train - train_mean)/train_std
	X_test = (X_test - test_mean)/test_std
	"""
	
	print(name)
	for clf_name, clf in zip(names, classifiers):
		clf.fit(X_train, y_train)
		score = clf.score(X_test, y_test)
		score_txt = str(round(score*100, 2))
		result = "{:<16}{:<16}".format(clf_name, score_txt)
		print(result)
	print()
	
if __name__ == "__main__":
	classification("hututu", "../results/bosphorus/hututu-Neutrals.dat", False)
	classification("hu1980", "../results/bosphorus/hu1980-Neutrals.dat", False)
	classification("hututu_no-outliers", "../results/bosphorus_no-outliers/hututu-Neutrals.dat", False)
	classification("hu1980_no-outliers", "../results/bosphorus_no-outliers/hu1980-Neutrals.dat", False)
	
