"""
classification.py
Artur Rodrigues Rocha Neto
2018

Funções para classificação, análise de resultados e pré-processamento.
Os arquivos de dados com as features/atributos devem estar no formato:
- p+4 colunas, onde p é o número de atributos
- as quatros colunas finais devem ser, respectivamente:
  - amostra
  - classe
  - tipo de expressao
  - expressao
- os arquivos NÃO DEVEM POSSUIR CABEÇALHO!
- exemplos:
	1,2,3,4,5,6,9,7,N,N # seis atributos, amostra 9, classe 7, neutra-neutra
	9,2,0,2,1,2,E,ANGER # quatro atributos, amostra 1, classe 2, expressao-anger
	3,4,9,LFAU,A22B22   # um atributo, amostra 4, classe 9, actionunit-A22B22
"""

import os
import time
import warnings
import operator
import itertools
import numpy as np
import pandas as pd
import seaborn as sn
import matplotlib.pyplot as plt
from sklearn.svm import SVM
from sklearn.naive_bayes import GaussianNB
from sklearn.metrics import confusion_matrix
from sklearn.ensemble import AdaBoostClassifier
from sklearn.tree import DecisionTreeClassifier
from sklearn.preprocessing import StandardScaler
from sklearn.preprocessing import PowerTransformer
from sklearn.ensemble import RandomForestClassifier
from sklearn.neighbors import NearestCentroid as NC
from sklearn.neighbors import KNeighborsClassifier as KNN
from sklearn.discriminant_analysis import LinearDiscriminantAnalysis as LDA
from sklearn.discriminant_analysis import QuadraticDiscriminantAnalysis as QDA

warnings.filterwarnings("ignore")

classifiers = {
	"KNN_manhattam" : KNN(p=1, n_neighbors=1),
	"KNN_euclidean" : KNN(p=2, n_neighbors=1),
	"DMC_manhattam" : NC(metric="manhattan"),
	"DMC_euclidean" : NC(metric="euclidean"),
	"SVM_radial"    : SVM(kernel="rbf", gamma="auto"),
	"SVM_poly"      : SVM(kernel="poly", gamma="auto"),
	"LDA"           : LDA(),
	"QDA"           : QDA(),
	"GaussianNB"    : GaussianNB(),
	"RandomForest"  : RandomForestClassifier(),
	"AdaBoost"      : AdaBoostClassifier(),
	"DecisionTree"  : DecisionTreeClassifier()
}

def run_classification(X_train, y_train, X_test, y_test):
	"""
	Roda os classificadores em estudo para um dado conjunto de treino e teste.
	Essa é a função mais genérica e mais importante desse script. Performa
	normalização e remoção de viês.

	:param X_train: amostras de treino
	:param y_train: classes das amostras de treino
	:param X_test: amostras de teste
	:param y_test: classes das amostras de teste
	:return: dicionário com taxas de reconhecimento e predições
	"""
	
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

def rank1_neutral(features):
	"""
	Executa classificação RANK-1 usando os classificadores em pesquisa. Treino:
	amostra 0 de cada pose neutra. Teste: restante das amostras neutras
	(amostra diferente de 0).

	:param features: caminho para o dados extraídos das amostras neutras
	:return: dicionário com taxas de reconhecimento e predições
	"""
	
	df = pd.read_csv(features, header=None)
	cols = ["f"+str(x) for x in range(len(df.columns)-4)]
	cols = cols + ["sample", "subject", "tp", "exp"]
	df.columns = cols
	
	trainset = df.loc[df["sample"] == 0].drop(["sample", "tp", "exp"], axis=1)
	X_train = np.array(trainset.drop(["subject"], axis=1))
	y_train = np.ravel(trainset[["subject"]])
	
	testset = df.loc[df["sample"] != 0].drop(["sample", "tp", "exp"], axis=1)
	X_test = np.array(testset.drop(["subject"], axis=1))
	y_test = np.ravel(testset[["subject"]])
	
	return run_classification(X_train, y_train, X_test, y_test)

def rank1_nonneutral(feat_neutral, feat_nonneutral):
	"""
	Executa classificação RANK-1 usando os classificadores em pesquisa. Treino:
	amostra 0 de cada pose neutra. Teste: todas as amostras não-neutras.

	:param feat_neutral: caminho para os dados das amostras neutras
	:param feat_nonneutral: caminho para os dados das amostras não-neutras
	:return: dicionário com taxas de reconhecimento e predições
	"""
	
	# neutras: conjunto de treino
	df = pd.read_csv(feat_neutral, header=None)
	cols = ["f"+str(x) for x in range(len(df.columns)-4)]
	cols = cols + ["sample", "subject", "tp", "exp"]
	df.columns = cols
	
	trainset = df.loc[df["sample"] == 0].drop(["sample", "tp", "exp"], axis=1)
	X_train = np.array(trainset.drop(["subject"], axis=1))
	y_train = np.ravel(trainset[["subject"]])
	
	# nao-neutras: conjunto de teste
	df = pd.read_csv(feat_nonneutral, header=None)
	cols = ["f"+str(x) for x in range(len(df.columns)-4)]
	cols = cols + ["sample", "subject", "tp", "exp"]
	df.columns = cols
	
	testset = df.drop(["sample", "tp", "exp"], axis=1)
	X_test = np.array(testset.drop(["subject"], axis=1))
	y_test = np.ravel(testset[["subject"]])
	
	return run_classification(X_train, y_train, X_test, y_test)

def roc1(feat_neutral, feat_nonneutral):
	"""
	Executa classificação ROC-1 usando os classificadores em pesquisa. Treino:
	todas as amostras neutras. Teste: todas as amostras não-neutras.

	:param feat_neutral: caminho para os dados das amostras neutras
	:param feat_nonneutral: O caminho para os dados das amostras não-neutras
	:return: dicionário com taxas de reconhecimento e predições
	"""
	
	# neutras: conjunto de treino
	df = pd.read_csv(feat_neutral, header=None)
	cols = ["f"+str(x) for x in range(len(df.columns)-4)]
	cols = cols + ["sample", "subject", "tp", "exp"]
	df.columns = cols
	
	trainset = df.drop(["sample", "tp", "exp"], axis=1)
	X_train = np.array(trainset.drop(["subject"], axis=1))
	y_train = np.ravel(trainset[["subject"]])
	
	# nao-neutras: conjunto de teste
	df = pd.read_csv(feat_nonneutral, header=None)
	cols = ["f"+str(x) for x in range(len(df.columns)-4)]
	cols = cols + ["sample", "subject", "tp", "exp"]
	df.columns = cols
	
	testset = df.drop(["sample", "tp", "exp"], axis=1)
	X_test = np.array(testset.drop(["subject"], axis=1))
	y_test = np.ravel(testset[["subject"]])
	
	return run_classification(X_train, y_train, X_test, y_test)

def confusion(y_true, y_pred, classes=[]):
	"""
	Monta matriz de confusão para um resultado de classificação.

	:param y_true: verdade terrestre
	:param y_pred: classes estimadas
	:param classes: As classes do problema
	"""
	
	conf = confusion_matrix(y_true, y_pred, labels=classes)
	sn.heatmap(conf, cmap="Purples")
	
	tick_marks = np.arange(len(classes))
	plt.xticks(tick_marks, classes, rotation=90)
	plt.yticks(tick_marks, classes, rotation=360)
	
	plt.title("Matriz de Confusão")
	plt.ylabel("Verdade terrestre")
	plt.xlabel("Classes estimadas")
	plt.tight_layout()
	plt.show()

def max_rate(ans):
	"""
	Separa a melhor taxa de acerto de uma saída de classificação.

	:param ans: resposta de classificação no formato de dicionário
	:return: classificador e taxa
	"""
	
	rates = dict((key, value["recog"]) for key, value in ans.items())
	lemax = max(rates.items(), key=operator.itemgetter(1))
	
	return lemax[0], lemax[1]

def max_confusion(ans):
	"""
	Monta uma matriz com as predições e os reais da maior taxa de acerto de uma
	resposta de classificação.

	:param ans: resposta de classificação no formato de dicionário
	"""
	
	classifier, rate = max_rate(ans)
	diff = np.array([ans[classifier]["y_true"], ans[classifier]["y_pred"]]).T
	for i in range(diff.shape[0]):
		if diff[i, 0] != diff[i, 1]:
			print("[{}]:\t{}".format(i, diff[i,:]))

