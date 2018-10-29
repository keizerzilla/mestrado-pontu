"""
classification.py
Artur Rodrigues Rocha Neto
2018

Funções para classificação, análise de resultados e pré-processamento.
Os arquivos de dados com as features/atributos devem estar no formato:

- p+2 colunas, onde p é o número de atributos
- as duas colunas finais devem ser, respectivamente, amostra e classe
- os arquivos NÃO DEVEM POSSUIR CABEÇALHO!
- exemplo:
	
	1,2,3,4,5,6,9,7 # seis atributos, amostra 9, classe 7
	8,4,9,2,0,2,1,2 # seis atributos, amostra 1, classe 2
	3,9,1,5,0,3,4,9 # seis atributos, amostra 4, classe 9
	(...)
	
Fonte1: http://scikit-learn.org/stable/modules/model_evaluation.html
Fonte2: http://www.zvetcobiometrics.com/Support/definitions.php
"""

import time
import operator
import itertools
import numpy as np
import pandas as pd
import seaborn as sn
import matplotlib.pyplot as plt
from sklearn.svm import SVC
from sklearn.metrics import confusion_matrix
from sklearn.model_selection import LeaveOneOut
from sklearn.preprocessing import StandardScaler
from sklearn.model_selection import GridSearchCV
from sklearn.preprocessing import PowerTransformer
from sklearn.neighbors import NearestCentroid as NC
from sklearn.neural_network import MLPClassifier as MLP
from sklearn.neighbors import KNeighborsClassifier as KNC

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
é a função mais genérica e mais importante desse script. Performa normalização e
remoção de viês.

X_train -- amostras de treino
y_train -- classes das amostras de treino
X_test -- amostras de teste
y_test -- classes das amostras de teste

return -- Dicionário com taxa de reconhecimento e predições das classificações
"""
def run_classification(X_train, y_train, X_test, y_test):
	# normalizacao
	scaler = StandardScaler().fit(X_train)
	X_train = scaler.transform(X_train)
	X_test = scaler.transform(X_test)
	
	# remocao de skewness (precisa testar mais)
	new_train = []
	new_test = []
	for m_train, m_test in zip(X_train.T, X_test.T):
		data_train = m_train.reshape(-1, 1)
		data_test = m_test.reshape(-1, 1)
		pt = PowerTransformer(method="yeo-johnson", standardize=True)
		pt.fit(data_train)
		new_train.append(np.ravel(pt.transform(data_train)))
		new_test.append(np.ravel(pt.transform(data_test)))
	
	X_train = np.array(new_train).T
	X_test = np.array(new_test).T
	
	# execucao dos classificadores e registro dos resultados
	ans = dict()
	for name, classifier in zip(names, classifiers):
		result = dict()
		
		start_time = time.time()
		clf = classifier
		clf.fit(X_train, y_train)
		score = clf.score(X_test, y_test)
		elapsed_time = round(time.time() - start_time, 4)
		
		result["recog"] = score
		result["y_true"] = y_test
		result["y_pred"] = clf.predict(X_test)
		ans[name] = result
		
		score_txt = str(round(score*100, 2))
		out = "{:<16}{:<8}{:<8}".format(name, score_txt, elapsed_time)
		print(out)
	
	return ans

"""
Executa classificação RANK-1 usando os classificadores em pesquisa. Treino:
amostra 0 de cada pose neutra. Teste: restante das amostras neutras
(amostra diferente de 0).

name -- O nome do teste em execução
features -- O caminho para o dados extraídos das amostras neutras

return -- Dicionário com taxa de reconhecimento e predições das classificações
"""
def rank1_neutral(name, features):
	print(name)
	
	df = pd.read_csv(features, header=None)
	cs = ["f"+str(x) for x in range(len(df.columns)-2)] + ["sample", "subject"]
	df.columns = cs
	
	trainset = df.loc[df["sample"] == 0].drop(["sample"], axis=1)
	X_train = np.array(trainset.drop(["subject"], axis=1))
	y_train = np.ravel(trainset[["subject"]])
	
	testset = df.loc[df["sample"] != 0].drop(["sample"], axis=1)
	X_test = np.array(testset.drop(["subject"], axis=1))
	y_test = np.ravel(testset[["subject"]])
	
	# execucao dos classificadores
	return run_classification(X_train, y_train, X_test, y_test)

"""
Executa classificação RANK-1 usando os classificadores em pesquisa. Classes com
apenas uma amostra não são usadas! Treino: amostra 0 de cada pose neutra. Teste:
restante das amostras neutras (amostra diferente de 0).

name -- O nome do teste em execução
features -- O caminho para o dados extraídos das amostras neutras

return -- Dicionário com taxa de reconhecimento e predições das classificações
"""
def rank1_neutral_easy(name, features):
	print(name)
	
	df = pd.read_csv(features, header=None)
	cs = ["f"+str(x) for x in range(len(df.columns)-2)] + ["sample", "subject"]
	df.columns = cs
	
	# APENAS CLASSES COM MAIS DE UMA AMOSTRA
	unique, counts = np.unique(df["subject"], return_counts=True)
	ocurrences = dict(zip(unique, counts))
	more = [key for key, val in ocurrences.items() if val > 1]
	df = df[df["subject"].isin(more)]
	
	trainset = df.loc[df["sample"] == 0].drop(["sample"], axis=1)
	X_train = np.array(trainset.drop(["subject"], axis=1))
	y_train = np.ravel(trainset[["subject"]])
	
	testset = df.loc[df["sample"] != 0].drop(["sample"], axis=1)
	X_test = np.array(testset.drop(["subject"], axis=1))
	y_test = np.ravel(testset[["subject"]])
	
	# execucao dos classificadores
	return run_classification(X_train, y_train, X_test, y_test)

"""
Executa classificação RANK-1 usando os classificadores em pesquisa. Treino:
amostra 0 de cada pose neutra. Teste: todas as amostras não-neutras.

name -- O nome do teste em execução
feat_neutral -- O caminho para o dados extraídos das amostras neutras
feat_nonneutral -- O caminho para os dados extraídos das amostras não-neutras

return -- Dicionário com taxa de reconhecimento e predições das classificações
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
	
	# execucao dos classificadores
	return run_classification(X_train, y_train, X_test, y_test)

"""
Executa classificação ROC-1 usando os classificadores em pesquisa. Treino: todas
as amostras neutras. Teste: todas as amostras não-neutras.

name -- O nome do teste em execução
feat_neutral -- O caminho para o dados extraídos das amostras neutras
feat_nonneutral -- O caminho para os dados extraídos das amostras não-neutras

return -- Dicionário com taxa de reconhecimento e predições das classificações
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
	
	# execucao dos classificadores
	return run_classification(X_train, y_train, X_test, y_test)

"""
Executa classificação RANK-1 usando os classificadores em pesquisa. O conjunto
de treino e teste é a concatenação de n momentos. Treino: amostra 0 de cada pose
neutra. Teste: restante das amostras neutras (amostra diferente de 0).

moments -- Lista de momentos

return -- Dicionário com taxa de reconhecimento e predições das classificações
"""
def rank1_concat(moments):
	x_train_list = []
	x_test_list = []
	y_train_list = []
	y_test_list = []
	for m in moments:
		df = pd.read_csv(m, header=None)
		cs = ["f"+str(x) for x in range(len(df.columns)-2)]
		cs = cs + ["sample", "subject"]
		df.columns = cs
		
		"""
		# APENAS CLASSES COM MAIS DE UMA AMOSTRA
		unique, counts = np.unique(df["subject"], return_counts=True)
		ocurrences = dict(zip(unique, counts))
		more = [key for key, val in ocurrences.items() if val > 1]
		df = df[df["subject"].isin(more)]
		"""
		
		trainset = df.loc[df["sample"] == 0].drop(["sample"], axis=1)
		temp_X_train = trainset.drop(["subject"], axis=1)
		temp_y_train = trainset[["subject"]]
		
		testset = df.loc[df["sample"] != 0].drop(["sample"], axis=1)
		temp_X_test = testset.drop(["subject"], axis=1)
		temp_y_test = testset[["subject"]]
		
		x_train_list.append(temp_X_train)
		x_test_list.append(temp_X_test)
		y_train_list.append(temp_y_train)
		y_test_list.append(temp_y_test)
	
	X_train = np.array(pd.concat(x_train_list, axis=1))
	X_test = np.array(pd.concat(x_test_list, axis=1))
	
	y_train = np.ravel(y_train_list[0])
	y_test = np.ravel(y_test_list[0])
	
	# executa classificadores
	return run_classification(X_train, y_train, X_test, y_test)

"""
Monta matriz de confusão para um resultado de classificação, plota e calcula
algumas métricas. Incompleta!!!

y_true -- Verdade terrestre
y_pred -- Classes estimadas
classes -- As classes do problema
"""
def confusion(y_true, y_pred):
	conf = confusion_matrix(y_true, y_pred)
	sn.heatmap(conf, cmap="Purples")
	plt.ylabel("Verdade terrestre")
	plt.xlabel("Classes estimadas")
	plt.tight_layout()
	plt.show()

