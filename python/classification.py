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
  - exepressao
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
from sklearn.svm import SVC
from sklearn.decomposition import PCA
from sklearn.naive_bayes import GaussianNB
from sklearn.metrics import confusion_matrix
from sklearn.ensemble import AdaBoostClassifier
from sklearn.tree import DecisionTreeClassifier
from sklearn.preprocessing import StandardScaler
from sklearn.preprocessing import PowerTransformer
from sklearn.ensemble import RandomForestClassifier
from sklearn.neighbors import NearestCentroid as NC
from sklearn.neural_network import MLPClassifier as MLP
from sklearn.neighbors import KNeighborsClassifier as KNC
from sklearn.model_selection import train_test_split as data_split
from sklearn.discriminant_analysis import LinearDiscriminantAnalysis as LDA
from sklearn.discriminant_analysis import QuadraticDiscriminantAnalysis as QDA

expressions = {
	"ANGER"    : 0,
	"HAPPY"    : 1,
	"SURPRISE" : 2,
	"SADNESS"  : 3,
	"DISGUST"  : 4,
	"FEAR"     : 5
}

warnings.filterwarnings("ignore")

classifiers = {
	"KNN_manhattam" : KNC(p=1, n_neighbors=1),
	"KNN_euclidean" : KNC(p=2, n_neighbors=1),
	"DMC_manhattam" : NC(metric="manhattan"),
	"DMC_euclidean" : NC(metric="euclidean"),
	"SVM_radial"    : SVC(kernel="rbf", gamma="auto"),
	"SVM_poly"      : SVC(kernel="poly", gamma="auto"),
	"LDA"           : LDA(),
	"QDA"           : QDA(),
	"GaussianNB"    : GaussianNB(),
	"RandomForest"  : RandomForestClassifier(),
	"AdaBoost"      : AdaBoostClassifier(),
	"DecisionTree"  : DecisionTreeClassifier()
}

def run_classification(X_train, y_train, X_test, y_test, norm=True, skew=True):
	"""
	Roda os classificadores em estudo para um dado conjunto de treino e teste.
	Essa é a função mais genérica e mais importante desse script. Performa
	normalização e remoção de viês.

	:param X_train: amostras de treino
	:param y_train: classes das amostras de treino
	:param X_test: amostras de teste
	:param y_test: classes das amostras de teste
	:param norm: se deve aplicar normalizacao no conjunto
	:param skew: se deve remover skewness no conjunto
	:return: dicionário com taxas de reconhecimento e predições
	"""
	
	ans = dict()
	
	#print("antes:", X_train.shape, X_test.shape)
	X_train = X_train[:, ~np.all(np.equal(X_train, 0.0), axis=0)]
	X_test = X_test[:, ~np.all(np.equal(X_test, 0.0), axis=0)]
	#print("depois:", X_train.shape, X_test.shape)
	
	X_train[X_train == np.inf] = 0
	X_train[X_train == -np.inf] = 0
	X_train[X_train == np.nan] = 0
	X_test[X_test == np.inf] = 0
	X_test[X_test == -np.inf] = 0
	X_test[X_test == np.nan] = 0
	
	if norm:
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
	
	if skew:
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
	
	""" REMOCAO DE ALTAS CORRELACOES - INUTIL (por enquanto...)
	corr = df.drop(["sample", "subject", "tp", "exp"], axis=1).corr().abs()
	upper = corr.where(np.triu(np.ones(corr.shape), k=1).astype(np.bool))
	to_drop = [column for column in upper.columns if any(upper[column] >= 0.9)]
	df = df.drop(to_drop, axis=1)
	"""
	
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

def expression_classification(feat_nonneutral):
	"""
	Executa classificação de expressões usando os classificadores em pesquisa.
	Treino: amostra 0 de cada pose neutra. Teste: todas as amostras não-neutras.
	
	:param feat_nonneutral: caminho para os dados das amostras não-neutras
	:return: dicionário com taxas de reconhecimento e predições
	"""
	
	df = pd.read_csv(feat_nonneutral, header=None)
	cols = ["f"+str(x) for x in range(len(df.columns)-4)]
	cols = cols + ["sample", "subject", "tp", "exp"]
	df.columns = cols
	
	df = df.loc[df["tp"] == "E"].drop(["subject", "sample", "tp"], axis=1)
	df = df.replace({"exp" : expressions})
	
	X = np.array(df.drop(["exp"], axis=1))
	y = np.ravel(df[["exp"]])
	
	rounds = 100
	results = []
	for i in range(rounds):
		X_train, X_test, y_train, y_test = data_split(X, y, test_size=0.7)
		ans = run_classification(X_train, y_train, X_test, y_test)
		results.append(ans)
	

def rank1_neutral_concat(moments):
	"""
	Executa classificação RANK-1 neutral usando os classificadores em pesquisa.
	O conjunto de treino e teste é a concatenação de n momentos. Treino: amostra
	0 de cada pose neutra. Teste: restante das amostras neutras (amostras
	diferentes de 0).

	:param moments: lista ou tupla de momentos
	:return: dicionário com taxas de reconhecimento e predições
	"""
	
	x_train_list = []
	x_test_list = []
	y_train_list = []
	y_test_list = []
	for m in moments:
		df = pd.read_csv(m, header=None)
		cols = ["f"+str(x) for x in range(len(df.columns)-4)]
		cols = cols + ["sample", "subject", "tp", "exp"]
		df.columns = cols
		
		trainset = df.loc[df["sample"] == 0].drop(["sample","tp","exp"], axis=1)
		temp_X_train = trainset.drop(["subject"], axis=1)
		temp_y_train = trainset[["subject"]]
		
		testset = df.loc[df["sample"] != 0].drop(["sample","tp","exp"], axis=1)
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
	
	return run_classification(X_train, y_train, X_test, y_test)

def rank1_nonneutral_concat(neutral, nonneutral):
	"""
	Executa classificação RANK-1 nonneutral usando os classificadores em
	pesquisa. O conjunto de treino e teste é a concatenação de n momentos.
	Treino: amostra 0 de cada pose neutra. Teste: restante das amostras neutras
	(amostras diferentes de 0).

	:param neutral: lista ou tupla de momentos neutrals
	:param nonneutral: lista ou tupla de momentos nonneutrals
	:return: dicionário com taxas de reconhecimento e predições
	"""
	
	x_train_list = []
	y_train_list = []
	for m in neutral:
		df = pd.read_csv(m, header=None)
		cols = ["f"+str(x) for x in range(len(df.columns)-4)]
		cols = cols + ["sample", "subject", "tp", "exp"]
		df.columns = cols
		
		trainset = df.loc[df["sample"] == 0].drop(["sample","tp","exp"], axis=1)
		temp_X_train = trainset.drop(["subject"], axis=1)
		temp_y_train = trainset[["subject"]]
		
		x_train_list.append(temp_X_train)
		y_train_list.append(temp_y_train)
	
	x_test_list = []
	y_test_list = []
	for m in nonneutral:
		df = pd.read_csv(m, header=None)
		cols = ["f"+str(x) for x in range(len(df.columns)-4)]
		cols = cols + ["sample", "subject", "tp", "exp"]
		df.columns = cols
		
		testset = df.drop(["sample", "tp", "exp"], axis=1)
		temp_X_test = testset.drop(["subject"], axis=1)
		temp_y_test = testset[["subject"]]
		
		x_test_list.append(temp_X_test)
		y_test_list.append(temp_y_test)
	
	X_train = np.array(pd.concat(x_train_list, axis=1))
	X_test = np.array(pd.concat(x_test_list, axis=1))
	
	y_train = np.ravel(y_train_list[0])
	y_test = np.ravel(y_test_list[0])
	
	return run_classification(X_train, y_train, X_test, y_test)

def roc1_concat(neutral, nonneutral):
	"""
	Executa classificação ROC1 usando os classificadores em pesquisa. O conjunto
	de treino e teste é a concatenação de n momentos.
	Treino: todas as amostras neutras. Teste: todas as amostras não-neutras
	(amostras diferentes de 0).

	:param neutral: lista ou tupla de momentos neutrals
	:param nonneutral: lista ou tupla de momentos nonneutrals
	:return: dicionário com taxas de reconhecimento e predições
	"""
	
	x_train_list = []
	y_train_list = []
	for m in neutral:
		df = pd.read_csv(m, header=None)
		cols = ["f"+str(x) for x in range(len(df.columns)-4)]
		cols = cols + ["sample", "subject", "tp", "exp"]
		df.columns = cols
		
		trainset = df.drop(["sample", "tp", "exp"], axis=1)
		temp_X_train = trainset.drop(["subject"], axis=1)
		temp_y_train = trainset[["subject"]]
		
		x_train_list.append(temp_X_train)
		y_train_list.append(temp_y_train)
	
	x_test_list = []
	y_test_list = []
	for m in nonneutral:
		df = pd.read_csv(m, header=None)
		cols = ["f"+str(x) for x in range(len(df.columns)-4)]
		cols = cols + ["sample", "subject", "tp", "exp"]
		df.columns = cols
		
		testset = df.drop(["sample", "tp", "exp"], axis=1)
		temp_X_test = testset.drop(["subject"], axis=1)
		temp_y_test = testset[["subject"]]
		
		x_test_list.append(temp_X_test)
		y_test_list.append(temp_y_test)
	
	X_train = np.array(pd.concat(x_train_list, axis=1))
	X_test = np.array(pd.concat(x_test_list, axis=1))
	
	y_train = np.ravel(y_train_list[0])
	y_test = np.ravel(y_test_list[0])
	
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

def combination_rank1_neutral(dataset, moments, cuts, n=2, dump="../results/"):
	"""
	Executa classificações exaustivas do cenário rank1a para uma lista de
	combinações de momentos.
	
	:param dataset: conjunto de dados base a ser testado
	:param moments: lista de momentos a ser usados
	:param n: número de elementos por combinação (default 2)
	:param dump: pasta aonde ficarao salvas as combinacoes
	"""
	
	extension = ".dat"
	basepath = dump
	slices = [v+"/" for k, v in cuts.items()]
	
	moments = [dataset + "/neutral-" + m + extension for m in moments]
	products = list(itertools.product(slices, moments))
	configs = [basepath + p[0] + p[1] for p in products]
	combos = list(itertools.combinations(configs, n))
	settings = [[(c[i].split("/")[2],
	              os.path.basename(c[i]).split("-")[1].replace(".dat",""))
	              for i in range(n)]
	              for c in combos]
	
	print("total de combinacoes:", len(settings))
	
	cols = ["setting", "classifier", "rate"]
	df = pd.DataFrame(columns=cols)
	
	for setting, combo in zip(settings, combos):
		ans = rank1_neutral_concat(combo)
		classifier, rate = max_rate(ans)
		rate = round(rate*100, 2)
		setting = str(setting)
		result = "[{}] [{}] [{}]".format(setting, classifier, rate)
		row = {"setting" : setting, "classifier" : classifier, "rate" : rate}
		df = df.append(row, ignore_index=True)
		print(result)
	
	res_path = "{}combination_rank1a_P{}/".format(dump, n)
	os.makedirs(res_path, exist_ok=True)
	df.to_csv(res_path + "{}.csv".format(dataset), index=False)

def combination_rank1_nonneutral(dataset, moments, cuts, n=2, dump="../results/"):
	"""
	Executa classificações exaustivas do cenário rank1b para uma lista de
	combinações de momentos.
	
	:param dataset: conjunto de dados base a ser testado
	:param moments: lista de momentos a ser usados
	:param n: número de elementos por combinação (default 2)
	:param dump: pasta aonde ficarao salvas as combinacoes
	"""
	
	extension = ".dat"
	basepath = dump
	slices = [v+"/" for k, v in cuts.items()]
	
	m_neutral = [dataset + "/neutral-" + m + extension for m in moments]
	m_nonneutral = [dataset + "/nonneutral-" + m + extension for m in moments]
	
	prod_neutral = list(itertools.product(slices, m_neutral))
	prod_nonneutral = list(itertools.product(slices, m_nonneutral))
	
	conf_neutral = [basepath + p[0] + p[1] for p in prod_neutral]
	conf_nonneutral = [basepath + p[0] + p[1] for p in prod_nonneutral]
	
	combo_neutral = list(itertools.combinations(conf_neutral, n))
	combo_nonneutral = list(itertools.combinations(conf_nonneutral, n))
	
	settings = [[(c[i].split("/")[2],
	              os.path.basename(c[i]).split("-")[1].replace(".dat",""))
	              for i in range(n)]
	              for c in combo_neutral]
	
	print("total de combinacoes:", len(settings))
	
	cols = ["setting", "classifier", "rate"]
	df = pd.DataFrame(columns=cols)
	data_zip = zip(settings, combo_neutral, combo_nonneutral)
	
	for setting, neutral, nonneutral in data_zip:
		ans = rank1_nonneutral_concat(neutral, nonneutral)
		classifier, rate = max_rate(ans)
		rate = round(rate*100, 2)
		setting = str(setting)
		result = "[{}] [{}] [{}]".format(setting, classifier, rate)
		row = {"setting" : setting, "classifier" : classifier, "rate" : rate}
		df = df.append(row, ignore_index=True)
		print(result)
	
	res_path = "{}combination_rank1b_P{}/".format(dump, n)
	os.makedirs(res_path, exist_ok=True)
	df.to_csv(res_path + "{}.csv".format(dataset), index=False)

def combination_roc1(dataset, moments, cuts, n=2, dump="../results/"):
	"""
	Executa classificações exaustivas do cenário roc1 para uma lista de
	combinações de momentos.
	
	:param dataset: conjunto de dados base a ser testado
	:param moments: lista de momentos a ser usados
	:param n: número de elementos por combinação (default 2)
	:param dump: pasta aonde ficarao salvas as combinacoes
	"""
	
	extension = ".dat"
	basepath = dump
	slices = [v+"/" for k, v in cuts.items()]
	
	m_neutral = [dataset + "/neutral-" + m + extension for m in moments]
	m_nonneutral = [dataset + "/nonneutral-" + m + extension for m in moments]
	
	prod_neutral = list(itertools.product(slices, m_neutral))
	prod_nonneutral = list(itertools.product(slices, m_nonneutral))
	
	conf_neutral = [basepath + p[0] + p[1] for p in prod_neutral]
	conf_nonneutral = [basepath + p[0] + p[1] for p in prod_nonneutral]
	
	combo_neutral = list(itertools.combinations(conf_neutral, n))
	combo_nonneutral = list(itertools.combinations(conf_nonneutral, n))
	
	settings = [[(c[i].split("/")[2],
	              os.path.basename(c[i]).split("-")[1].replace(".dat",""))
	              for i in range(n)]
	              for c in combo_neutral]
	
	print("total de combinacoes:", len(settings))
	
	cols = ["setting", "classifier", "rate"]
	df = pd.DataFrame(columns=cols)
	data_zip = zip(settings, combo_neutral, combo_nonneutral)
	
	for setting, neutral, nonneutral in data_zip:
		ans = roc1_concat(neutral, nonneutral)
		classifier, rate = max_rate(ans)
		rate = round(rate*100, 2)
		setting = str(setting)
		result = "[{}] [{}] [{}]".format(setting, classifier, rate)
		row = {"setting" : setting, "classifier" : classifier, "rate" : rate}
		df = df.append(row, ignore_index=True)
		print(result)
	
	res_path = "{}combination_roc1_P{}/".format(dump, n)
	os.makedirs(res_path, exist_ok=True)
	df.to_csv(res_path + "{}.csv".format(dataset), index=False)

