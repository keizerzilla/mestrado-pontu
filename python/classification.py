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
from sklearn.preprocessing import StandardScaler
from sklearn.preprocessing import PowerTransformer
from sklearn.neighbors import NearestCentroid as NC
from sklearn.neural_network import MLPClassifier as MLP
from sklearn.neighbors import KNeighborsClassifier as KNC
from sklearn.discriminant_analysis import LinearDiscriminantAnalysis as LDA

classifiers = [
	KNC(p=1, n_neighbors=1),
	KNC(p=2, n_neighbors=1),
	NC(metric="manhattan"),
	NC(metric="euclidean"),
	SVC(kernel="rbf", gamma="auto"),
	SVC(kernel="poly", gamma="auto"),
	MLP(solver='lbfgs', alpha=1e-5, hidden_layer_sizes=(220,),
	    activation='tanh', random_state=1),
	LDA()]

names = [
	"KNN_manhattam",
	"KNN_euclidean",
	"DMC_manhattam",
	"DMC_euclidean",
	"SVM_radial",
	"SVM_poly",
	"MLP",
	"LDA"]

def run_classification(X_train, y_train, X_test, y_test, verbose=False):
	"""
	Roda os classificadores em estudo para um dado conjunto de treino e teste.
	Essa é a função mais genérica e mais importante desse script. Performa
	normalização e remoção de viês.

	:param X_train: amostras de treino
	:param y_train: classes das amostras de treino
	:param X_test: amostras de teste
	:param y_test: classes das amostras de teste
	:param verbose: se o resultado deve ser mostrado ou não na saída padrão
	:return: dicionário com taxas de reconhecimento e predições
	"""
	
	ans = dict()
	
	# normalizacao
	try:
		scaler = StandardScaler().fit(X_train)
		X_train = scaler.transform(X_train)
		X_test = scaler.transform(X_test)
	except:
		result = dict()
		result["recog"] = 0
		result["y_true"] = []
		result["y_pred"] = []
		ans["ERROR"] = result
		return ans
	
	# remocao de skewness
	new_train = []
	new_test = []
	pt = PowerTransformer(method="yeo-johnson", standardize=False) # antes True
	for m_train, m_test in zip(X_train.T, X_test.T):
		data_train = m_train.reshape(-1, 1)
		data_test = m_test.reshape(-1, 1)
		pt.fit(data_train)
		new_train.append(np.ravel(pt.transform(data_train)))
		new_test.append(np.ravel(pt.transform(data_test)))
	
	X_train = np.array(new_train).T
	X_test = np.array(new_test).T
	
	# execucao dos classificadores e registro dos resultados
	for name, classifier in zip(names, classifiers):
		result = dict()
		clf = classifier
		
		start_time = time.time()
		
		try:
			clf.fit(X_train, y_train)
			score = clf.score(X_test, y_test)
		except:
			score = 0
		
		result["recog"] = score
		result["y_true"] = y_test
		
		try:
			result["y_pred"] = clf.predict(X_test)
		except:
			result["y_pred"] = []
		
		elapsed_time = round(time.time() - start_time, 4)
		ans[name] = result
		
		if verbose:
			score_txt = str(round(score*100, 2))
			out = "{:<16}{:<8}{:<8}".format(name, score_txt, elapsed_time)
			print(out)
	
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

def rank1_neutral_easy(features):
	"""
	Executa classificação RANK-1 usando os classificadores em pesquisa. Classes
	com apenas uma amostra não são usadas! Treino: amostra 0 de cada pose
	neutra. Teste: restante das amostras neutras (amostra diferente de 0).

	:param features: caminho para o dados extraídos das amostras neutras
	:param return: dicionário com taxas de reconhecimento e predições
	"""
	
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

def rank1_concat(moments):
	"""
	Executa classificação RANK-1 usando os classificadores em pesquisa. O
	conjunto de treino e teste é a concatenação de n momentos. Treino: amostra 0
	de cada pose neutra. Teste: restante das amostras neutras (amostra diferente
	de 0).

	:param moments: lista ou tupla de momentos
	:return: dicionário com taxas de reconhecimento e predições
	"""
	
	x_train_list = []
	x_test_list = []
	y_train_list = []
	y_test_list = []
	for m in moments:
		df = pd.read_csv(m, header=None)
		cs = ["f"+str(x) for x in range(len(df.columns)-2)]
		cs = cs + ["sample", "subject"]
		df.columns = cs
		
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

def combination(dataset, n=2):
	"""
	Executa classificações exaustivas para uma lista de combinações de momentos.
	
	:param dataset: conjunto de dados base a ser testado
	:param n: número de elementos por combinação
	"""
	
	basepath = "../results/"
	
	slices = ["frontal/",
	          "radial/",
	          "sagittal/",
	          "transversal/",
	          "whole/",
	          "upper/"]
	
	moments = ["neutral-hu1980.dat",
		       "neutral-hututu.dat",
		       "neutral-legendre.dat",
		       "neutral-chebyshev.dat",
		       "neutral-zernike.dat"]
	
	moments = [dataset + "/" + m for m in moments]
	products = list(itertools.product(slices, moments))
	configs = [basepath + p[0] + p[1] for p in products]
	combos = list(itertools.combinations(configs, n))
	settings = [[(c[i].split("/")[2], os.path.basename(c[i])) for i in range(n)]
	            for c in combos]
	
	cols = ["setting", "classifier", "rate"]
	df = pd.DataFrame(columns=cols)
	
	for setting, combo in zip(settings, combos):
		classifier, rate = max_rate(rank1_concat(combo))
		rate = round(rate*100, 2)
		setting = str(setting)
		result = "[{}] [{}] [{}]".format(setting, classifier, rate)
		row = {"setting" : setting, "classifier" : classifier, "rate" : rate}
		df = df.append(row, ignore_index=True)
		print(result)
	
	res_path = "../results/combination{}/".format(n)
	os.makedirs(res_path, exist_ok=True)
	
	df.to_csv(res_path + "{}.csv".format(dataset), index=False)

