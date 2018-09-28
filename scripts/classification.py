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
	...
	
"""

"""
Executa classificação RANK-1 usando os 4 classificadores em pesquisa. Treino:
amostra 0 de cada pose neutra. Teste: restante das amostras neutras. Uma
normalização padrão (centro na médida e escala no desvio padrão) é computada no
conjunto de dados.

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
	
	df = pd.read_csv(features, header=None)
	
	cs = ["f"+str(x) for x in range(len(df.columns)-2)] + ["sample", "subject"]
	df.columns = cs
	
	trainset = df.loc[df["sample"] == 0].drop(["sample"], axis=1)
	X_train = np.array(trainset.drop(["subject"], axis=1))
	y_train = np.ravel(trainset[["subject"]])
	
	testset = df.loc[df["sample"] != 0].drop(["sample"], axis=1)
	X_test = np.array(testset.drop(["subject"], axis=1))
	y_test = np.ravel(testset[["subject"]])
	
	scaler = StandardScaler().fit(X_train)
	X_train = scaler.transform(X_train)
	X_test = scaler.transform(X_test)
	
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

# !!! rank1_nonneutral AQUI !!!

if __name__ == "__main__":
	rank1_neutral("hututu", "../results/moments/hututu-Neutrals.dat")
	rank1_neutral("legendre", "../results/moments/legendre-Neutrals.dat")
	rank1_neutral("tchebychev", "../results/moments/tchebychev-Neutrals.dat")
	rank1_neutral("zernike", "../results/moments/zernike-Neutrals.dat")

