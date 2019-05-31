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

# @TODO
"""
Debug para função de registro.
Plota duas nuvens em função de uma transformação.
"""
def draw_registration_result(source, target, transformation):
	source_temp = copy.deepcopy(source)
	target_temp = copy.deepcopy(target)
	source_temp.paint_uniform_color([1, 0.706, 0])
	target_temp.paint_uniform_color([0, 0.651, 0.929])
	source_temp.transform(transformation)
	draw_geometries([source_temp, target_temp])

# @TODO
"""
Tarefa de registro. Efetua operação em uma base completa.
A ideia é reproduzir fielmanete o pipeline do Robson. Sem sucesso até o momento.
"""
def registration(input_clouds, input_lm3, output_clouds, output_lm3, f_all):
	fmt = "bs{:d}_{:w}_{:w}_{:d}.pcd"
	os.makedirs(output_clouds, exist_ok=True)
	os.makedirs(output_lm3, exist_ok=True)
	os.makedirs(f_all, exist_ok=True)
	
	for bsdir in os.listdir(input_clouds):
		folder = input_clouds + bsdir
		nose_path = input_lm3 + bsdir
		result_folder = output_clouds + bsdir
		lm3_folder = output_lm3 + bsdir
		
		if os.path.isdir(folder):
			os.makedirs(result_folder, exist_ok=True)
			os.makedirs(lm3_folder, exist_ok=True)
			
			for cloud in os.listdir(folder):
				match = parse.parse(fmt, cloud)
				tp = str(match[1])
				sample = str(match[3])
				
				if cloud.endswith(".pcd") and tp == "N":
					ref_cloud = folder + "/" + bsdir + "_N_N_0.pcd"
					ref_nose = nose_path + "/" + bsdir + "_N_N_0.pcd"
					fullPath = folder + "/" + cloud
					fullLm3 = nose_path + "/" + cloud
					fullResult = result_folder + "/" + cloud
					lm3Result = lm3_folder + "/" + cloud
					
					cloud_neutral0 = read_point_cloud(ref_cloud)
					lm3_neutral0 = read_point_cloud(ref_nose)
					cloud_target = read_point_cloud(fullPath)
					lm3_target = read_point_cloud(fullLm3)
					
					points = np.asarray(cloud_neutral0.points)
					nosetip = np.reshape(lm3_neutral0.points[13], (1, 3))
					condition = cdist(points, nosetip) <= 40
					nose_points = points[list(np.ravel(condition))]
					nose_cloud = PointCloud()
					nose_cloud.points = Vector3dVector(nose_points)
					
					trans_init = np.array([[1., 0., 0., 0.],
					                       [0., 1., 0., 0.],
					                       [0., 0., 1., 0.],
					                       [0., 0., 0., 1.]])
					point2point = TransformationEstimationPointToPoint()
					criteria = ICPConvergenceCriteria(relative_fitness=0.0001,
					                                  relative_rmse=0.0001,
					                                  max_iteration=100)
					threshold = 100
					reg = registration_icp(nose_cloud,
					                       cloud_target,
					                       threshold,
					                       trans_init,
					                       point2point,
					                       criteria=criteria)
					
					cloud_copy = copy.deepcopy(cloud_target)
					lm3_copy = copy.deepcopy(lm3_target)
					
					cloud_copy.transform(reg.transformation)
					lm3_copy.transform(reg.transformation)
					
					write_point_cloud(fullResult, cloud_copy, write_ascii=True)
					write_point_cloud(lm3Result, lm3_copy, write_ascii=True)
					
					fullAll = f_all + cloud
					all_copy = np.asarray(cloud_copy.points)
					np.savetxt(fullAll.replace("pcd", "xyz"),
					           all_copy,
					           delimiter=" ")
					
					print(cloud, "ok!")

"""
Extração momentos.
Toma uma nuvem alvo, a nuvem de landmarks e o tipo de fatiamento.
"""
def cloud_extraction(cloud, nose, cut, fmt="bs{:d}_{:w}_{:w}_{:d}.pcd"):
	mcalc = "../bin/siqcalc"
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

"""
Executa extração de momentos em uma base completa.
"""
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

"""
Executa extração de momentos em uma base completa.
"""
def batch_extraction_allinone(dataset, cut, output):
	print("[  siqueira  ] - [ {} ] - [{}]".format(cut, dataset))
	
	fmt="bs{:d}_{:w}_{:w}_{:d}.xyz"
	count = 0
	start_time = time.time()
	
	dump = []
	for cloud in os.listdir(dataset):
		if cloud.endswith(".xyz"):
			fullPath = dataset + "/" + cloud
			
			ans = cloud_extraction(fullPath, "vruco.pcd", cut, fmt)
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
	
