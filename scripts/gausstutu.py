import os
import parse
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit
from scipy.stats import linregress
from sklearn import preprocessing
from sklearn.svm import SVC
from sklearn.neighbors import KNeighborsClassifier
from sklearn.naive_bayes import GaussianNB

# funcao-modelo da gaussiana
def gauss(x, A, mu, sigma):
	return A*np.exp(-0.5*np.power(((x-mu)/sigma), 2))

# retorna dados da base
def get_data(path):
	return np.array(pd.read_csv(path, header=None))

# calcula fitting
def fitting(source):
	# definindo dados
	data = get_data(source)
	hist, bin_edges = np.histogram(data, density=False)
	bin_centres = (bin_edges[:1] + bin_edges[1:])/2
	
	# gerando fit dos dados
	p0 = [1, 30, 10]
	coeff, var_matrix = curve_fit(gauss, bin_centres, hist, p0=p0)
	hist_fit = gauss(bin_centres, coeff[0], coeff[1], coeff[2])
	
	# plotando resultados
	return bin_centres, hist, hist_fit, coeff

# exibicao de resultados caso preciso
def gauss_plot(bin_centres, hist, hist_fit, coeff):
	# plotando resultados
	plt.plot(bin_centres, hist, label="TEST DATA", color="C1")
	plt.plot(bin_centres, hist_fit, label="FITTED DATA", color="C2")
	
	# exibindo parametros
	print("MEDIA DO FIT = " + str(coeff[1]))
	print("DESVIO DO FIT = " + str(coeff[2]))
	
	# monta janelas
	plt.show()

# analise em batch da base
def gauss_batch(folder, dest):
	parse_format = "bs{:d}_{}_{}_{:d}.txt"
	
	with open(dest, "w") as dump:
		header = "id,sample,mu,sigma\n"
		dump.write(header)
		for cloud in os.listdir(folder):
			cloud_info = parse.parse(parse_format, cloud)
			subject = str(cloud_info[0])
			sample = str(cloud_info[3])
			source = folder+cloud
			
			bin_centres, hist, hist_fit, coeff = fitting(source)
			mu = str(coeff[1])
			sigma = str(coeff[2])
			
			to_file = subject+","+sample+","+mu+","+sigma+"\n"
			dump.write(to_file)
			
			print(cloud + " OK")

# extracao dos conjuntos principais em batch
def bosphorus():
	gauss_batch("../centerprofile/neutral_sort/", "/home/artur/neutral.txt")
	gauss_batch("../centerprofile/nonneutral_sort/", "/home/artur/non.txt")

# plota valores de media e variancia
def plotter(mu, sigma, title):
	plt.plot(mu, label="MEDIA")
	plt.plot(sigma, label="VARIANCIA")
	plt.legend()
	plt.title(title)
	plt.show()

# analise dos resultados
def analizer():
	sources = ["../gaussian/neutral.txt", "../gaussian/nonneutral.txt"]
	
	for source in sources:
		data = pd.read_csv(source)
		mu = np.array(data[["mu"]])
		sigma = np.array(data[["sigma"]]) # desvio
		sigma = np.power(sigma, 2) # variancia
		
		mean = np.mean(mu)
		var = np.mean(sigma)
		
		print("{} > mean = {} | var = {}".format(source, mean, var))

# classificacao show
def classification(features, source, header_save, file_save):
	data = pd.read_csv(source)
	
	X_train = np.array(data.loc[data["sample"] == 0, features])
	y_train = np.ravel(data.loc[data["sample"] == 0, ["id"]])
	
	X_test = np.array(data.loc[data["sample"] != 0, features])
	y_test = np.ravel(data.loc[data["sample"] != 0, ["id"]])
	
	# pre-processing
	X_train = preprocessing.scale(X_train)
	X_test = preprocessing.scale(X_test)
	#X_train = preprocessing.normalize(X_train)
	#X_test = preprocessing.normalize(X_test)
	
	classifiers = [
		KNeighborsClassifier(p=1, n_neighbors=1),
		KNeighborsClassifier(p=2, n_neighbors=1),
		SVC(kernel="rbf"),
		GaussianNB()]
	
	names = [
		"KNN_manhattam",
		"KNN_euclidean",
		"SVM_radial",
		"GaussianNB"]
	
	print(header_save)
	
	save = open(file_save, "a+")
	save.write(header_save + "\n")
	
	for clf_name, clf in zip(names, classifiers):
		clf.fit(X_train, y_train)
		score = clf.score(X_test, y_test)
		score_txt = str(round(score*100, 2))
		result = "{:<16}{:<16}".format(clf_name, score_txt)
		print(result)
		save.write(result + "\n")
	
	save.write("\n\n")
	save.close()

# analise estatistica
def gaussian(source):
	x = np.array(pd.read_csv(source, header=None))
	mu = np.mean(x)
	sigma = np.var(x)
	std = np.std(x)
	
	return mu, sigma, std

# analise linear
def linear(source, begin, end, plog):
	x = np.ravel(np.array(pd.read_csv(source, header=None)))
	
	logx = np.log2(x)
	logx = logx[np.where(logx >= plog)]
	logy = np.ravel(np.arange(logx.size))
	
	x = x[np.where(x >= begin)]
	x = x[np.where(x <= end)]
	y = np.ravel(np.arange(x.size))
	
	slopelin, intlin, r_value, p_value, std_err = linregress(x, y)
	slopelog, intlog, r_value, p_value, std_err = linregress(logx, logy)
	
	return slopelin, intlin, slopelog, intlog

# analise completa dos conjuntos de dados
def full_analysis(folder, dest, begin, end, plog):
	parse_format = "bs{:d}_{}_{}_{:d}.txt"
	
	print("ANALISANDO [{}]".format(folder))
	with open(dest, "w") as dump:
		header = "id,sample,mu,sigma,std,slopelin,intlin,slopelog,intlog\n"
		dump.write(header)
		for cloud in os.listdir(folder):
			cloud_info = parse.parse(parse_format, cloud)
			sub = str(cloud_info[0])
			sample = str(cloud_info[3])
			source = folder+cloud
			
			mu, sigma, std = gaussian(source)
			slopelin, intlin, slopelog, intlog = linear(source, begin, end, plog)
			
			to_file = "{},{},{},{},{},{},{},{},{}\n".format(sub, sample, mu, sigma, std, slopelin, intlin, slopelog, intlog)
			dump.write(to_file)
			#print(cloud + " OK")
	
# chamada da funcao principal
if __name__ == "__main__":
	src_neutral = "../centerprofile/neutral_sort/"
	dst_neutral = "/home/artur/fullneutral.txt"
	file_save = "/home/artur/neutral_results.txt"
	
	features = ["mu", "sigma", "slopelog", "intlog"]
	begin = 50.0
	end = 70.0
	plog = 4.0
	
	#for begin in range(0, 60, 10):
		#full_analysis(src_neutral, dst_neutral, begin, end, plog)
		#classification(features, dst_neutral, "({},{})|[{}]|{}".format(begin, end, plog, "-".join(features)), file_save)
	
	classification(features, dst_neutral, "({},{})|[{}]|{}".format(begin, end, plog, "-".join(features)), file_save)
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
