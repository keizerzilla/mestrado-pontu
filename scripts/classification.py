import time
import numpy as np
import pandas as pd
from sklearn.svm import SVC
from sklearn.preprocessing import StandardScaler
from sklearn.neighbors import KNeighborsClassifier as KNC

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

"""
Executa classificação RANK-1 usando os 4 classificadores em pesquisa.
Treino: amostra 0 de cada pose neutra.
Teste: restante das amostras neutras (amostra diferente de 0).
Uma normalização padrão (centro na médida e escala no desvio padrão) é computada
no conjunto de dados.

name -- O nome do teste em execução
features -- O caminho para o dados extraídos das amostras neutras
stdout -- Se o resultado da classificação deve ser exibido na saída padrão
"""
def rank1_neutral(name, features, stdout=True):
	if (stdout):
		print(name)
	
	classifiers = [
		KNC(p=1, n_neighbors=1),
		KNC(p=2, n_neighbors=1),
		SVC(kernel="rbf"),
		SVC(kernel="poly")]

	names = [
		"KNN_manhattam",
		"KNN_euclidean",
		"SVM_radial",
		"SVM_poly"]
	
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
	ans = dict()
	for clf_name, clf in zip(names, classifiers):
		start_time = time.time()
		
		clf.fit(X_train, y_train)
		score = clf.score(X_test, y_test)
		score_txt = str(round(score*100, 2))
		
		elapsed_time = round(time.time() - start_time, 4)
		
		if (stdout):
			out = "{:<16}{:<8}{:<8}".format(clf_name, score_txt, elapsed_time)
			print(out)
		
		ans[clf_name] = score
	
	return ans

"""
Executa classificação RANK-1 usando os 4 classificadores em pesquisa.
Treino: amostra 0 de cada pose neutra.
Teste: todas as amostras não-neutras.
Uma normalização padrão (centro na médida e escala no desvio padrão) é computada
no conjunto de dados.

name -- O nome do teste em execução
feat_neutral -- O caminho para o dados extraídos das amostras neutras
feat_nonneutral -- O caminho para os dados extraídos das amostras não-neutras
stdout -- Se o resultado da classificação deve ser exibido na saída padrão
"""
def rank1_nonneutral(name, feat_neutral, feat_nonneutral, stdout=True):
	if (stdout):
		print(name)
	
	classifiers = [
		KNC(p=1, n_neighbors=1),
		KNC(p=2, n_neighbors=1),
		SVC(kernel="rbf"),
		SVC(kernel="poly")]

	names = [
		"KNN_manhattam",
		"KNN_euclidean",
		"SVM_radial",
		"SVM_poly"]
	
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
	ans = dict()
	for clf_name, clf in zip(names, classifiers):
		start_time = time.time()
		
		clf.fit(X_train, y_train)
		score = clf.score(X_test, y_test)
		score_txt = str(round(score*100, 2))
		
		elapsed_time = round(time.time() - start_time, 4)
		
		if (stdout):
			out = "{:<16}{:<8}{:<8}".format(clf_name, score_txt, elapsed_time)
			print(out)
		
		ans[clf_name] = score
	
	return ans

"""
Executa classificação ROC-1 usando os 4 classificadores em pesquisa.
Treino: todas as amostras neutras.
Teste: todas as amostras não-neutras.
Uma normalização padrão (centro na médida e escala no desvio padrão) é computada
no conjunto de dados.

name -- O nome do teste em execução
feat_neutral -- O caminho para o dados extraídos das amostras neutras
feat_nonneutral -- O caminho para os dados extraídos das amostras não-neutras
stdout -- Se o resultado da classificação deve ser exibido na saída padrão
"""
def roc1(name, feat_neutral, feat_nonneutral, stdout=True):
	if (stdout):
		print(name)
	
	classifiers = [
		KNC(p=1, n_neighbors=1),
		KNC(p=2, n_neighbors=1),
		SVC(kernel="rbf"),
		SVC(kernel="poly")]

	names = [
		"KNN_manhattam",
		"KNN_euclidean",
		"SVM_radial",
		"SVM_poly"]
	
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
	ans = dict()
	for clf_name, clf in zip(names, classifiers):
		start_time = time.time()
		
		clf.fit(X_train, y_train)
		score = clf.score(X_test, y_test)
		score_txt = str(round(score*100, 2))
		
		elapsed_time = round(time.time() - start_time, 4)
		
		if (stdout):
			out = "{:<16}{:<8}{:<8}".format(clf_name, score_txt, elapsed_time)
			print(out)
		
		ans[clf_name] = score
	
	return ans

# verification rate

if __name__ == "__main__":
	"""
	rank1_neutral("hututu", "../results/moments/Neutrals-hututu.dat")
	rank1_neutral("legendre", "../results/moments/Neutrals-legendre.dat")
	rank1_neutral("tchebychev", "../results/moments/Neutrals-tchebychev.dat")
	rank1_neutral("zernike", "../results/moments/Neutrals-zernike.dat")
	"""
	
	"""
	rank1_nonneutral("hututu", "../results/moments/Neutrals-hututu.dat", "../results/moments/NonNeutrals-hututu.dat")
	rank1_nonneutral("legendre", "../results/moments/Neutrals-legendre.dat", "../results/moments/NonNeutrals-legendre.dat")
	rank1_nonneutral("tchebychev", "../results/moments/Neutrals-tchebychev.dat", "../results/moments/NonNeutrals-tchebychev.dat")
	rank1_nonneutral("zernike", "../results/moments/Neutrals-zernike.dat", "../results/moments/NonNeutrals-zernike.dat")
	"""
	
	"""
	roc1("hututu", "../results/moments/Neutrals-hututu.dat", "../results/moments/NonNeutrals-hututu.dat")
	roc1("legendre", "../results/moments/Neutrals-legendre.dat", "../results/moments/NonNeutrals-legendre.dat")
	roc1("tchebychev", "../results/moments/Neutrals-tchebychev.dat", "../results/moments/NonNeutrals-tchebychev.dat")
	roc1("zernike", "../results/moments/Neutrals-zernike.dat", "../results/moments/NonNeutrals-zernike.dat")
	"""
