"""
recognition.py
Artur Rodrigues Rocha Neto - artur.rodrigues26@gmail.com - 2019

Subconjunto de funções do pipeline de reconhecimento facial segundo reportado
por (SIQUEIRA;2018). Operações de registro, extração e classificação.

Requisitos: numpy, scipy, pandas, matplotlib, seaborn, parse, sklearn, open3d
"""

import os
import sys
import time
import copy
import parse
import seaborn
import operator
import subprocess
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from open3d import *
from sklearn.svm import SVC as SVM
from scipy.spatial.distance import cdist
from sklearn.metrics import confusion_matrix
from sklearn.preprocessing import StandardScaler

"""
Extração momentos.
Toma uma nuvem alvo e o tipo de fatiamento.
"""
def cloud_extraction(cloud, cut):
	fmt = "bs{:d}_{:w}_{:w}_{:d}.xyz"
	mcalc = "../bin/siqcalc"
	path, filename = os.path.split(cloud)
	match = parse.parse(fmt, filename)
	
	if match == None:
		return None
	
	subject = str(match[0])
	tp = str(match[1])
	exp = str(match[2])
	sample = str(match[3])
	
	cmd = [mcalc, "-i", cloud, "-o", "stdout", "-c", cut]
	ans = subprocess.run(cmd, stdout=subprocess.PIPE).stdout
	ans = ans[:-1].decode("utf-8").replace(" ", ",")
	ans = ans + ",{},{},{},{}\n".format(tp, exp, sample, subject)
	
	return ans

"""
Executa extração de momentos em uma base completa.
"""
def batch_extraction(dataset, cut, output):
	print("[  siqueira  ] - [ {} ] - [{}]".format(cut, dataset))
	
	count = 0
	start_time = time.time()
	
	dump = []
	for bsdir in os.listdir(dataset):
		folder = dataset + bsdir
		if os.path.isdir(folder):
			for cloud in os.listdir(folder):
				if cloud.endswith(".xyz"):
					fullPath = folder + "/" + cloud
					
					ans = cloud_extraction(fullPath, cut)
					if ans == None:
						continue
					
					dump.append(ans)
					count = count + 1
					print("\r{}".format(count), end="\r")
	
	elapsed = time.time() - start_time
	vel = round(elapsed / count, 6)
	
	cols = ["m" + str(x) for x in range(len(dump[0].split(",")) - 4)]
	cols = cols + ["tp", "exp", "sample", "subject"]
	cols = ",".join(cols) + "\n"
	dump = cols + "".join(dump)
	
	data_file = open(output, "w")
	data_file.write(dump)
	data_file.close()
	
	print("[     OK     ] - [ {} nuvens, {} seg/nuvem]".format(count, vel))

"""
Executa extração de momentos em uma base completa.
"""
def batch_extraction_allinone(dataset, cut, output):
	print("[  siqueira  ] - [ {} ] - [{}]".format(cut, dataset))
	
	count = 0
	start_time = time.time()
	
	dump = []
	for cloud in os.listdir(dataset):
		if cloud.endswith(".xyz"):
			fullPath = dataset + "/" + cloud
			
			ans = cloud_extraction(fullPath, cut)
			if ans == None:
				continue
			
			dump.append(ans)
			count = count + 1
			print("\r{}".format(count), end="\r")
	
	elapsed = time.time() - start_time
	vel = round(elapsed / count, 6)
	
	cols = ["m" + str(x) for x in range(len(dump[0].split(",")) - 4)]
	cols = cols + ["tp", "exp", "sample", "subject"]
	cols = ",".join(cols) + "\n"
	dump = cols + "".join(dump)
	
	data_file = open(output, "w")
	data_file.write(dump)
	data_file.close()
	
	print("[     OK     ] - [ {} nuvens, {} seg/nuvem]".format(count, vel))

"""
Função base de classificação.
Toma conjuntos de treino e teste, efetua normalização e executa classificador.
Retorno é um dicionário com os seguintes campos:

"recog": taxa de reconhecimento
"y_true": vetor com as classes verdadeiras
"y_pred": vetor com as classes preditas pelo classificador
"time": quanto tempo (segundos) demorou para rodar normalização + classificação
"""
def run_classification(X_train, y_train, X_test, y_test):
	X_train[X_train == np.inf] = 0
	X_train[X_train == -np.inf] = 0
	X_train[X_train == np.nan] = 0
	X_test[X_test == np.inf] = 0
	X_test[X_test == -np.inf] = 0
	X_test[X_test == np.nan] = 0
	
	start_time = time.time()
	
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
		return result
	
	classifier = SVM(kernel="rbf", C=8.0, gamma=0.125)
	result = dict()
	
	try:
		classifier.fit(X_train, y_train)
		score = classifier.score(X_test, y_test)
	except:
		print("! fit error !")
		score = 0
	
	result["recog"] = score
	result["y_true"] = y_test
	
	try:
		result["y_pred"] = classifier.predict(X_test)
	except:
		print("! predict error !")
		result["y_pred"] = []
	
	elapsed_time = str(round(time.time() - start_time, 4))
	result["time"] = elapsed_time
	
	return result

"""
Efetua cenário de classificação neutra.
Teste: amostra 0 neutra
Treino: todas as demais amostras neutras
"""
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

"""
Efetua cenário de classificação não-neutra.
Teste: amostra 0 neutra
Treino: todas as amostras não-neutras (expressões e AUs)
"""
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

"""
Efetua cenário de classificação roc3.
Teste: todas as amostras neutras
Treino: todas as amostras não-neutras (expressões e AUs)
"""
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

"""
Efetua cenário de classificação com oclusão.
Teste: amostra 0 neutra
Treino: todas as amostras de oclusão
"""
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

"""
Imprime taxa de reconhecimento em formato humanamente legível.
Recebe também um parâmetro opcional de mensagem de debug.
"""
def rocog_rate(ans, msg="RecogRate"):
	rate = round(ans["recog"]*100, 2)
	print("{:<16}{:<8}".format(msg, rate))

"""
Plota matriz de confusão de um dado experimento.
Printa na saída padrão uma lista dos erros para melhor visualização.
Recebe parâmetro opcional com título da figura.
"""
def plot_confusion_matrix(ans, title="reconhecimento"):
	seaborn.set()
	
	y_true = ans["y_true"]
	y_pred = ans["y_pred"]
	errors = 0
	
	print("VERDADE\t\tPREDICAO")
	
	for i in range(y_true.shape[0]):
		truth = y_true[i]
		pred = y_pred[i]
		
		if truth != pred:
			print("{}\t\t{}".format(truth, pred))
			errors = errors + 1
	
	print("{} errors".format(errors))
	
	classes = [x for x in range(105)]
	
	conf = confusion_matrix(y_true, y_pred, labels=classes)
	seaborn.heatmap(conf, cmap="Purples")
	
	tick_marks = np.arange(len(classes))
	plt.xticks(tick_marks, classes, rotation=90)
	plt.yticks(tick_marks, classes, rotation=360)
	
	plt.title("Matriz de Confusão: " + title)
	plt.ylabel("Verdade terrestre")
	plt.xlabel("Classes estimadas")
	plt.tight_layout()
	plt.show()

if __name__ == "__main__":
	dataset = "../datasets/bosphorus-robson/xyz/"
	result_folder = "../results/robson/"

	os.makedirs(result_folder, exist_ok=True)

	cuts = ["st"]
	for cut in cuts:
		output = result_folder + cut + "_data.dat"
		
		batch_extraction_allinone(dataset, cut, output)
		
		ans = rank_neutral(output)
		rocog_rate(ans, cut.upper() + "_neutral")
		ans = rank_nonneutral(output)
		rocog_rate(ans, cut.upper() + "_nonneutral")
		
