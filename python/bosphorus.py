"""
bosphorus.py
Artur Rodrigues Rocha Neto (artur.rodrigues26@gmail.com) - 2019

Funções para extração, classificação e análise de resultados na base Bosphorus.
Os arquivos de dados com os atributos devem estar no formato:
- p+4 colunas, onde p é o número de atributos
- as quatros colunas finais devem ser, respectivamente:
  > tipo de expressao
  > expressao
  > amostra
  > classe

Requisitos: numpy, scipy, pandas, matplotlib, seaborn, parse, sklearn
"""

import os
import copy
import time
import parse
import shutil
import warnings
import operator
import itertools
import subprocess
import numpy as np
import pandas as pd
import seaborn as sn
import matplotlib.pyplot as plt
from sklearn.svm import SVC as SVM
from sklearn.feature_selection import RFE
from sklearn.naive_bayes import GaussianNB
from sklearn.feature_selection import RFECV
from sklearn.metrics import confusion_matrix
from sklearn.ensemble import VotingClassifier
from sklearn.ensemble import AdaBoostClassifier
from sklearn.tree import DecisionTreeClassifier
from sklearn.model_selection import LeaveOneOut
from sklearn.preprocessing import StandardScaler
from sklearn.preprocessing import PowerTransformer
from sklearn.ensemble import RandomForestClassifier
from sklearn.neighbors import KNeighborsClassifier as KNN

### LISTA DE CLASSIFICADORES TESTADOS
classifiers = {
	"KNN_mht" : KNN(p=1, n_neighbors=1),
	"KNN_ecd" : KNN(p=2, n_neighbors=1),
	"SVM_rad" : SVM(kernel="rbf", C=8.0, gamma=0.125, probability=True),
	"SVM_lin" : SVM(kernel="linear", C=8.0, gamma=0.125, probability=True),
	"NvBayes" : GaussianNB(),
	"RandFor" : RandomForestClassifier(n_estimators=100),
	"AdBoost" : AdaBoostClassifier(),
	"DecTree" : DecisionTreeClassifier()
}

### FORMATO DO ARQUIVO BOSPHORUS
fmt = "bs{:d}_{:w}_{:w}_{:d}.xyz"

### CAMINHO PARA O EXECUTAVEL CALCULADOR DE MOMENTOS
mcalc = "../pontu/bin/mcalc"

### IGNORA WARNINGS DE CLASSIFICADORES CHATOS
warnings.filterwarnings("ignore")

def xyz2pcd(source, dest):
	"""Converte conjunto de nuvens .xyz em formato .pcd usando pcl_xyz2pcd.

	:param source: pasta com os arquivos a serem convertidos
	:param dest: pasta destino das nuvens convertidas
	"""
	
	for cloud in os.listdir(source):
		inpf = os.path.join(source, cloud)
		
		if not os.path.isfile(inpf) or not inpf.endswith(".xyz"):
			continue
		
		outf = os.path.join(dest, cloud.replace(".xyz", ".pcd"))
		cmd = ["pcl_xyz2pcd", inpf, outf]
		subprocess.call(cmd)
		print(outf + " OK")

def pcd2xyz(source, dest):
	"""Converte conjunto de nuvens .pcd em formato .xyz usando pcl_pcd2ply.

	:param source: pasta com os arquivos a serem convertidos
	:param dest: pasta destino das nuvens convertidas
	"""
	
	for cloud in os.listdir(source):
		inpf = os.path.join(source, cloud)
		
		if not os.path.isfile(inpf) or not inpf.endswith(".pcd"):
			continue
		
		outf = os.path.join(dest, cloud.replace(".pcd", ".ply"))
		cmd = ["pcl_pcd2ply", "-format", "0", "-use_camera", "0", inpf, outf]
		print(" ".join(cmd))
		subprocess.call(cmd)
		
		data = ""
		with open(outf, "r") as ply:
			data = ply.read()
			data = data[data.find("end_header")+11:]
		
		with open(outf, "w") as dump:
			dump.write(data)
		
		os.rename(outf, outf.replace(".ply", ".xyz"))
		
		print(outf + " OK")

def cloud_extraction(cloud, moment, cut):
	"""Extração de momentos de uma nuvem.
	
	:param cloud: Nuvem alvo
	:param moment: Momento a ser calculado
	:param cut: Tipo de corte
	:return: String com o vetor de atributos formatado
	"""
	
	path, filename = os.path.split(cloud)
	match = parse.parse(fmt, filename)
	
	if match == None:
		return None
	
	subject = str(match[0])
	tp = str(match[1])
	exp = str(match[2])
	sample = str(match[3])
	
	cmd = [mcalc, "-m", moment, "-i", cloud, "-o", "stdout", "-c", cut]
	ans = subprocess.run(cmd, stdout=subprocess.PIPE).stdout
	ans = ans[:-1].decode("utf-8").replace(" ", ",")
	ans = ans + ",{},{},{},{}\n".format(tp, exp, sample, subject)
	
	return ans

def batch_extraction(dataset, moment, cut, output):
	"""Extração de momentos em uma base completa.
	
	:param dataset: Diretório contendo todas as nuvens da base
	:param moment: Momento a ser calculado
	:param cut: Tipo de corte
	:param output: Arquivo de saída
	"""
	
	print("[ mcalc ] - [ {} ] - [ {} ] - [ {} ]".format(moment, cut, dataset))
	
	count = 0
	start_time = time.time()
	
	dump = []
	for cloud in os.listdir(dataset):
		if cloud.endswith(".xyz"):
			fullPath = dataset + "/" + cloud
			
			ans = cloud_extraction(fullPath, moment, cut)
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
	
	print("[ OK ] - [ {} nuvens, {} seg/nuvem ]".format(count, vel))

def run_classification(X_train, y_train, X_test, y_test, do_voting=False):
	"""Roda os classificadores em estudo para um dado conjunto de treino e
	teste. Essa é a função mais genérica e mais importante desse script. Aplica
	normalização e remoção de viês.

	:param X_train: amostras de treino
	:param y_train: classes das amostras de treino
	:param X_test: amostras de teste
	:param y_test: classes das amostras de teste
	:param do_voting: se deve ou não usar o comitê de classificadores (lento)
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
	
	ensemble = []
	for name, classifier in classifiers.items():
		ensemble.append((name, copy.copy(classifier)))
		
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
	
	if do_voting:
		for v in ["hard", "soft"]:
			voting = VotingClassifier(estimators=ensemble, voting=v, n_jobs=-1)
			voting = voting.fit(X_train, y_train)
			score = voting.score(X_test, y_test)
			
			result = dict()
			result["recog"] = score
			result["y_true"] = y_test
			result["y_pred"] = voting.predict(X_test)
			result["time"] = 0
			
			ans["voting_"+v] = result
		
	return ans

def rank_neutral(features):
	"""Cenário de classificação neutral vs neutral.
	
	:param features: Arquivo com os vetores de atributo
	:return: Dicionário com os resultados de classificação
	"""
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
	"""Cenário de classificação neutral vs non-neutral.
	
	:param features: Arquivo com os vetores de atributo
	:return: Dicionário com os resultados de classificação
	"""
	
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
	"""Cenário de classificação ROC3 (neutral vs non-neutral mais fácil).
	
	:param features: Arquivo com os vetores de atributo
	:return: Dicionário com os resultados de classificação
	"""
	
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
	"""Cenário de classificação neutral vs occlusion.
	
	:param features: Arquivo com os vetores de atributo
	:return: Dicionário com os resultados de classificação
	"""
	
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

def max_rate(ans):
	"""Separa a melhor taxa de acerto de uma saída de classificação.

	:param ans: Resposta de classificação no formato de dicionário
	:return: Classificador e taxa em formato mais legível
	"""
	
	rates = dict((key, value["recog"]) for key, value in ans.items())
	lemax = max(rates.items(), key=operator.itemgetter(1))
	
	return lemax[0], round(lemax[1]*100, 2)
	
def pipeline_extraction(dataset, moments, cuts, outfolder="../results/{}/"):
	"""Executa extração de atributos na base bosphorus
	
	:param dataset: conjunto de nuvens
	:param moments: os momentos que se quer extrair
	:param cuts: os tipos de corte
	:param outfolder: caminho para aonde os momentos serão salvos
	"""
	
	for cut in cuts:
		folder = outfolder.format(cut)
		os.makedirs(folder, exist_ok=True)
		for moment in moments:
			outfile = folder + moment + ".dat"
			batch_extraction(dataset, moment, cut, outfile)

def pipeline_classification(dataset, moments, cuts, outfolder="../results/{}/"):
	"""Executa classificação na base bosphorus
	
	:param dataset: conjunto de nuvens
	:param moments: os momentos que se quer extrair
	:param cuts: os tipos de corte
	:param outfolder: caminho para aonde os momentos forão salvos
	"""
	
	for cut in cuts:
		folder = outfolder.format(cut)
		os.makedirs(folder, exist_ok=True)
		for moment in moments:
			outfile = folder + moment + ".dat"
			ans = rank_neutral(outfile)
			classifier, rate = max_rate(ans)
			print("{}-{}\t{}\t{}".format(moment, cut, classifier, rate))

### MAIN FUNCTION
if __name__ == "__main__":
	dataset = "../datasets/bs-out-d200-c80-icp/"
	moments = ["zernike_odd"]
	cuts = ["w"]
	
	pipeline_extraction(dataset, moments, cuts)
	pipeline_classification(dataset, moments, cuts)
	
	
	
