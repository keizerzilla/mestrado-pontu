"""
recpad.py
Artur Rodrigues Rocha Neto
artur.rodrigues26@gmail.com
NOV/2018

Sobre: conjunto de funções criadas para a resolução dos trabalhos computacionais
Requisitos: Python 3.5+, numpy, pandas, matplotlib, scikit-learn, seaborn
"""

import sys
import warnings
import numpy as np
import pandas as pd
import seaborn as sb
import matplotlib.pyplot as plt
from sklearn.cluster import KMeans
from sklearn.cluster import k_means
from sklearn.decomposition import PCA
from sklearn.cluster import MiniBatchKMeans
from sklearn.metrics import silhouette_score
from sklearn.metrics import confusion_matrix
from sklearn.metrics import pairwise_distances
from sklearn.preprocessing import MinMaxScaler
from sklearn.preprocessing import StandardScaler
from sklearn.metrics import davies_bouldin_score
from sklearn.preprocessing import PowerTransformer
from sklearn.metrics import calinski_harabaz_score
from sklearn.neighbors import NearestCentroid as DMC
from sklearn.metrics.pairwise import euclidean_distances
from sklearn.neighbors import KNeighborsClassifier as NN
from sklearn.model_selection import train_test_split as data_split
from sklearn.discriminant_analysis import LinearDiscriminantAnalysis as LDA
from sklearn.discriminant_analysis import QuadraticDiscriminantAnalysis as CQG

warnings.filterwarnings("ignore")

def heatmap(df, title):
	corr = df.corr().abs()
	annot = corr.round(decimals=2)
	upper = corr.where(np.triu(np.ones(corr.shape), k=1).astype(np.bool))
	to_drop = [c for c in upper.columns if any(upper[c] >= 0.9)]
	super_corr = [c for c in upper.columns if any(upper[c] >= 0.98)]
	print("drops sugeridos: " + str(to_drop))
	print("super correlacoes: " + str(super_corr))
	
	sb.heatmap(corr, vmin=0, vmax=1.0, cmap="inferno", annot=annot)
	plt.title(title)
	plt.show()

def data_stats(df):
	mean = df.mean()
	median = df.median()
	minv = df.min()
	maxv = df.max()
	std = df.std()
	
	result = pd.concat([mean, median, minv, maxv, std], axis=1)
	result.columns = ["Média", "Mediana", "Min", "Max", "Desvio"]
	result = result.round(2)
	
	return result

def pairplot(df, title):
	sb.pairplot(df, markers="o", palette="GnBu_d", diag_kind="kde",
		        plot_kws=dict(s=50, edgecolor="b", linewidth=1))
	plt.subplots_adjust(left=0.06, bottom=0.07, right=0.98, top=0.92,
	                    wspace=0.18, hspace=0.18)
	plt.suptitle(title)
	plt.show()

def piechart(df, class_col, labels, title):
	num = df[class_col].value_counts()
	print(num)
	num.plot.pie(labels=labels, autopct="%1.1f%%")
	plt.title(title)
	plt.ylabel("")
	plt.show()

def histogram(df, title):
	df.hist()
	plt.suptitle(title)
	plt.subplots_adjust(0.16, 0.06, 0.86, 0.90, 0.30, 0.68)
	plt.show()

def rank_covmatrix(X):
	X_data = np.array(X)
	covm = np.cov(X_data.T)
	rank_covm = np.linalg.matrix_rank(covm)
	
	return rank_covm

def reduction_kmeans(X, n=1000):
	X_new, _, inertia = k_means(X, n_clusters=n, n_jobs=-1)
	
	return X_new, inertia

def clustering_kmeans(X, n):
	#km = KMeans(n_clusters=n, n_init=50, max_iter=2000, n_jobs=-1)
	km = MiniBatchKMeans(n_clusters=n, n_init=50, max_iter=2000)
	km = km.fit(X)
	labels = km.labels_
	
	ch = calinski_harabaz_score(X, labels)
	db = davies_bouldin_score(X, labels)
	dunn = silhouette_score(X, labels)
	
	return ch, db, dunn, labels, km.cluster_centers_

def reduction_pca(X, y=None, n=None):
	pca = PCA(n_components=n)
	pca.fit(X)
	X = pca.transform(X)
	
	return X

def reduction_lda(X, y, n=None):
	pca = LDA(n_components=n)
	pca.fit(X, y)
	X = pca.transform(X)
	
	return X

def sensitivity(tp, fn):
	return tp / (tp + fn)

def specificity(tn, fp):
	return tn / (tn + fp)

def super_normalize(X):
	scaler = StandardScaler()
	scaler.fit(X)
	X = scaler.transform(X)
	
	return X

def super_unskew(X):
	transformer = PowerTransformer()
	transformer.fit(X)
	X = transformer.transform(X)
	
	return X

def minmax_scaling(X):
	scaler = MinMaxScaler()
	scaler.fit(X)
	X = scaler.transform(X)
	
	return X

def do_normalize(X_train, X_test):
	scaler = StandardScaler()
	scaler.fit(X_train)
	X_train = scaler.transform(X_train)
	X_test = scaler.transform(X_test)
	
	return X_train, X_test

def do_skewremoval(X_train, X_test):
	transformer = PowerTransformer()
	transformer.fit(X_train)
	X_train = transformer.transform(X_train)
	X_test = transformer.transform(X_test)
	
	return X_train, X_test

def progress(count, total, status=''):
	bar_len = 50
	filled_len = int(round(bar_len * count / float(total)))
	
	percents = round(100.0 * count / float(total), 1)
	bar = "=" * filled_len + "-" * (bar_len - filled_len)
	
	sys.stdout.write("[%s] %s%s %s\r" % (bar, percents, "%", status))
	sys.stdout.flush()

def classify(classifiers, X, y, test_size, rounds, verbose=False,
                                                   normalize=False,
                                                   unskew=False):
	
	ans = {key: {"score" : [], "sens" : [], "spec" : []}
	       for key, value in classifiers.items()}
	
	for i in range(rounds):
		X_train, X_test, y_train, y_test = data_split(X, y, test_size=test_size)
		for name, classifier in classifiers.items():
			if normalize:
				X_train, X_test = do_normalize(X_train, X_test)
			
			if unskew:
				X_train, X_test = do_skewremoval(X_train, X_test)
			
			classifier.fit(X_train, y_train)
			score = classifier.score(X_test, y_test)
			y_pred = classifier.predict(X_test)
			#confmatrix = confusion_matrix(y_test, y_pred)
			#tn, fp, fn, tp = confmatrix.ravel()
			sens = 0
			spec = 0
			
			ans[name]["score"].append(score)
			ans[name]["sens"].append(sens)
			ans[name]["spec"].append(spec)
			
			if verbose:
				progress(i+1, rounds)
	
	return ans

def find_best_pca(dataset, classifiers, test_rate, save_plot, verbose=False,
                                                              normalize=False,
                                                              unskew=False):
	df = pd.read_csv(dataset)
	df = df.drop(["name"], axis=1)
	X = df.drop(["status"], axis=1)
	y = df["status"]
	
	data = {"NN" : [], "DMC" : [], "CQG" : []}
	anses = []
	vector = range(1, len(X.columns)+1)
	for n in vector:
		X_red = reduction_pca(X, y, n)
		ans = classify(classifiers, X_red, y, test_rate, 100, verbose,
		                                                      normalize,
		                                                      unskew)
		anses.append(ans)
		
		data["NN"].append(round(np.mean(ans["NN"]["score"])*100, 2))
		data["DMC"].append(round(np.mean(ans["DMC"]["score"])*100, 2))
		data["CQG"].append(round(np.mean(ans["CQG"]["score"])*100, 2))
	
	labels = [str(n) for n in vector]
	df = pd.DataFrame.from_dict(data)
	
	ax = df.plot()
	plt.xticks(np.arange(len(X.columns)+1), labels=labels)
	plt.suptitle("Evolução da precisão em função do PCA")
	plt.xlabel("Número de Componentes")
	plt.ylabel("Precisão (%)")
	plt.ylim((65.0, 95.0))
	plt.legend(loc='upper center', bbox_to_anchor=(0.5, 1.09), ncol=3)
	
	for d in data:
		max_v = max(data[d])
		n = data[d].index(max_v)
		new_ans = {d : anses[n][d]}
		sumary(new_ans, "PCA: classif {}, q={}, max={}".format(d, n+1, max_v))
	
	plt.savefig(save_plot)

def sumary(ans, title="Vizualizando resposta de classificacao"):
	size = 70
	separator = "-"
	
	print(separator*size)
	print("SUMARY: {}".format(title))
	print(separator*size)
	print("CLASSIF\t\tMEAN\tMEDIAN\tMINV\tMAXV\tSTD\tSENS\tSPEC")
	print(separator*size)
	
	for n in ans:
		mean = round(np.mean(ans[n]["score"])*100, 2)
		median = round(np.median(ans[n]["score"])*100, 2)
		minv = round(np.min(ans[n]["score"])*100, 2)
		maxv = round(np.max(ans[n]["score"])*100, 2)
		std = round(np.std(ans[n]["score"])*100, 2)
		sens = round(np.mean(ans[n]["sens"])*100, 2)
		spec = round(np.mean(ans[n]["spec"])*100, 2)
		print("{}\t\t{}\t{}\t{}\t{}\t{}\t{}%\t{}%".format(n, mean, median, minv,
		                                                  maxv, std, sens,
		                                                  spec))
	
	print(separator*size)
	print()
	
