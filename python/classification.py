import time
import operator
import numpy as np
import pandas as pd
from sklearn.svm import SVC
from sklearn.model_selection import LeaveOneOut
from sklearn.preprocessing import StandardScaler
from sklearn.model_selection import GridSearchCV
from sklearn.neighbors import KNeighborsClassifier as KNC
from sklearn.neighbors import NearestCentroid as NC

"""
classification.py
Artur Rodrigues Rocha Neto
2018

Funções para classificação, análise de resultados e pré-processamento.
Os arquivos de dados com as features/atributos deve estar no formato:

- p+2 colunas, onde p é o número de atributos
- duas colunas finais devem ser, respectivamente, id da amostra e id da classe
- os arquivos NÃO DEVEM POSSUIR CABEÇAÇHO, pois os códigos já esperam o formato
- exemplo:
	
	1,2,3,4,5,6,9,7 # seis atributos, amostra 9, classe 7
	8,4,9,2,0,2,1,2 # seis atributos, amostra 1, classe 2
	3,9,1,5,0,3,4,9 # seis atributos, amostra 4, classe 9
	(...)
	
Fonte1: http://scikit-learn.org/stable/modules/model_evaluation.html
Fonte2: http://www.zvetcobiometrics.com/Support/definitions.php
"""

classifiers = [
	KNC(p=1, n_neighbors=1),
	KNC(p=2, n_neighbors=1),
	NC(metric="manhattan"),
	NC(metric="euclidean"),
	SVC(kernel="rbf", gamma="auto"),
	SVC(kernel="poly", gamma="auto")]

names = [
	"KNN_manhattam",
	"KNN_euclidean",
	"DMC_manhattam",
	"DMC_euclidean",
	"SVM_radial",
	"SVM_poly"]

grids = [
	{
	  "p"           : [1, 2],
	  "metric"      : ["chebyshev", "minkowski", "manhattan"],
	  "weights"     : ["uniform", "distance"],
	  "algorithm"   : ["ball_tree", "kd_tree", "brute"],
	  "leaf_size"   : [1, 5, 10, 15, 20, 25, 30, 35, 40],
	  "n_neighbors" : [1, 2, 3]
	},
	{
	  "C"                       : [x for x in range(1, 11)],
	  "tol"                     : [1e-4, 1e-3, 1e-2, 1e-1, 1e0],
	  "gamma"                   : [0.125, 0.25, 0.375, 0.5, 0.625, 0.75, 0.875],
	  "coef0"                   : [0.0, 0.1, 0.2, 0.3, 0.4, 0.5],
	  "kernel"                  : ["linear", "rbf", "poly", "sigmoid"],
	  "degree"                  : [2, 3, 4, 5],
	  "shrinking"               : [True, False],
	  "probability"             : [True, False],
	  "decision_function_shape" : ["ovo", "ovr"]
	}
]

"""
Roda os classificadores em estudo para um dado conjunto de treino e teste. Essa
é a função mais genérica e mais importante desse script.

X_train -- amostras de treino
y_train -- classes das amostras de treino
X_test -- amostras de teste
y_test -- classes das amostras de teste
"""
def run_classification(X_train, y_train, X_test, y_test):
	ans = dict()
	
	for name, classifier in zip(names, classifiers):
		start_time = time.time()
		clf = classifier
		clf.fit(X_train, y_train)
		score = clf.score(X_test, y_test)
		elapsed_time = round(time.time() - start_time, 4)
		ans[name] = score
		
		score_txt = str(round(score*100, 2))
		out = "{:<16}{:<8}{:<8}".format(name, score_txt, elapsed_time)
		print(out)
	
	return ans

"""
Executa classificação RANK-1 usando os classificadores em pesquisa.
Treino: amostra 0 de cada pose neutra.
Teste: restante das amostras neutras (amostra diferente de 0).
Uma normalização padrão (centro na médida e escala no desvio padrão) é computada
no conjunto de dados.

name -- O nome do teste em execução
features -- O caminho para o dados extraídos das amostras neutras
"""
def rank1_neutral(name, features):
	print(name)
	
	# neutras: teste e treino
	df = pd.read_csv(features, header=None)
	cs = ["f"+str(x) for x in range(len(df.columns)-2)] + ["sample", "subject"]
	df.columns = cs
	
	trainset = df.loc[df["sample"] == 0].drop(["sample"], axis=1)
	X_train = np.array(trainset.drop(["subject"], axis=1))
	y_train = np.ravel(trainset[["subject"]])
	
	testset = df.loc[df["sample"] != 0].drop(["sample"], axis=1)
	X_test = np.array(testset.drop(["subject"], axis=1))
	y_test = np.ravel(testset[["subject"]])
	
	# normalizacao
	scaler = StandardScaler().fit(X_train)
	X_train = scaler.transform(X_train)
	X_test = scaler.transform(X_test)
	
	# execucao dos classificadores
	return run_classification(X_train, y_train, X_test, y_test)

"""
Executa classificação RANK-1 usando os classificadores em pesquisa.
Treino: amostra 0 de cada pose neutra.
Teste: todas as amostras não-neutras.
Uma normalização padrão (centro na médida e escala no desvio padrão) é computada
no conjunto de dados.

name -- O nome do teste em execução
feat_neutral -- O caminho para o dados extraídos das amostras neutras
feat_nonneutral -- O caminho para os dados extraídos das amostras não-neutras
"""
def rank1_nonneutral(name, feat_neutral, feat_nonneutral):
	print(name)
	
	# neutras: conjunto de treino
	df = pd.read_csv(feat_neutral, header=None)
	cs = ["f"+str(x) for x in range(len(df.columns)-2)] + ["sample", "subject"]
	df.columns = cs
	
	trainset = df.loc[df["sample"] == 0].drop(["sample"], axis=1)
	X_train = np.array(trainset.drop(["subject"], axis=1))
	y_train = np.ravel(trainset[["subject"]])
	
	# nao-neutras: conjunto de teste
	df = pd.read_csv(feat_nonneutral, header=None)
	cs = ["f"+str(x) for x in range(len(df.columns)-2)] + ["sample", "subject"]
	df.columns = cs
	
	testset = df.drop(["sample"], axis=1)
	X_test = np.array(testset.drop(["subject"], axis=1))
	y_test = np.ravel(testset[["subject"]])
	
	# normalizacao
	scaler = StandardScaler().fit(X_train)
	X_train = scaler.transform(X_train)
	X_test = scaler.transform(X_test)
	
	# execucao dos classificadores
	return run_classification(X_train, y_train, X_test, y_test)

"""
Executa classificação ROC-1 usando os classificadores em pesquisa.
Treino: todas as amostras neutras.
Teste: todas as amostras não-neutras.
Uma normalização padrão (centro na médida e escala no desvio padrão) é computada
no conjunto de dados.

name -- O nome do teste em execução
feat_neutral -- O caminho para o dados extraídos das amostras neutras
feat_nonneutral -- O caminho para os dados extraídos das amostras não-neutras
"""
def roc1(name, feat_neutral, feat_nonneutral):
	print(name)
	
	# neutras: conjunto de treino
	df = pd.read_csv(feat_neutral, header=None)
	cs = ["f"+str(x) for x in range(len(df.columns)-2)] + ["sample", "subject"]
	df.columns = cs
	
	trainset = df.drop(["sample"], axis=1)
	X_train = np.array(trainset.drop(["subject"], axis=1))
	y_train = np.ravel(trainset[["subject"]])
	
	# nao-neutras: conjunto de teste
	df = pd.read_csv(feat_nonneutral, header=None)
	cs = ["f"+str(x) for x in range(len(df.columns)-2)] + ["sample", "subject"]
	df.columns = cs
	
	testset = df.drop(["sample"], axis=1)
	X_test = np.array(testset.drop(["subject"], axis=1))
	y_test = np.ravel(testset[["subject"]])
	
	# normalizacao
	scaler = StandardScaler().fit(X_train)
	X_train = scaler.transform(X_train)
	X_test = scaler.transform(X_test)
	
	# execucao dos classificadores
	return run_classification(X_train, y_train, X_test, y_test)

"""
Executa classificação RANK-1 usando os classificadores em pesquisa. O conjunto
de treino e teste é a concatenação de dois momentos.
Treino: amostra 0 de cada pose neutra.
Teste: restante das amostras neutras (amostra diferente de 0).
Uma normalização padrão (centro na médida e escala no desvio padrão) é computada
no conjunto de dados.

m1 -- Primeiro momento
m2 -- Segundo momento
"""
def rank1_duo(m1, m2):
	# momento #1
	
	df = pd.read_csv(m1, header=None)
	cs = ["f"+str(x) for x in range(len(df.columns)-2)] + ["sample", "subject"]
	df.columns = cs
	
	trainset = df.loc[df["sample"] == 0].drop(["sample"], axis=1)
	m1_X_train = trainset.drop(["subject"], axis=1)
	m1_y_train = trainset[["subject"]]
	
	testset = df.loc[df["sample"] != 0].drop(["sample"], axis=1)
	m1_X_test = testset.drop(["subject"], axis=1)
	m1_y_test = testset[["subject"]]
	
	# momento #2
	
	df = pd.read_csv(m2, header=None)
	cs = ["f"+str(x) for x in range(len(df.columns)-2)] + ["sample", "subject"]
	df.columns = cs
	
	trainset = df.loc[df["sample"] == 0].drop(["sample"], axis=1)
	m2_X_train = trainset.drop(["subject"], axis=1)
	m2_y_train = trainset[["subject"]]
	
	testset = df.loc[df["sample"] != 0].drop(["sample"], axis=1)
	m2_X_test = testset.drop(["subject"], axis=1)
	m2_y_test = testset[["subject"]]
	
	# concatena
	
	X_train = pd.concat([m1_X_train, m2_X_train], axis=1)
	X_test = pd.concat([m1_X_test, m2_X_test], axis=1)
	
	X_train = np.array(X_train)
	X_test = np.array(X_test)
	y_train = np.ravel(m1_y_train) # tanto faz um ou outro
	y_test = np.ravel(m1_y_test) # tanto faz um ou outro
	
	# normaliza
	
	scaler = StandardScaler().fit(X_train)
	X_train = scaler.transform(X_train)
	X_test = scaler.transform(X_test)
	
	# executa classificadores
	
	return run_classification(X_train, y_train, X_test, y_test)



if __name__ == "__main__":
	scenarios = ["bosphorus",
	             "bosphorus-outlier",
	             "bosphorus-outlier-densit200",
	             "bosphorus-outlier-densit200-crop60",
	             "bosphorus-outlier-densit200-crop70",
	             "bosphorus-outlier-densit200-crop80",
	             "bosphorus-outlier-densit200-crop60-icp",
	             "bosphorus-outlier-densit200-crop70-icp",
	             "bosphorus-outlier-densit200-crop80-icp",
	             "bosphorus-outlier-densit225",
	             "bosphorus-outlier-densit225-crop60",
	             "bosphorus-outlier-densit225-crop70",
	             "bosphorus-outlier-densit225-crop80",
	             "bosphorus-outlier-densit225-crop60-icp",
	             "bosphorus-outlier-densit225-crop70-icp",
	             "bosphorus-outlier-densit225-crop80-icp"]
	moments = ["zernike"]
	datasets = ["../results/" + x + "/" for x in scenarios]
	
	for data in datasets:
		print(data)
		for moment in moments:
			rank1_neutral(moment, data + "neutral-{}.dat".format(moment))
	
	
	
	
	
	
	
	
