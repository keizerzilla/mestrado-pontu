import os
import parse
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit

# funcao-modelo da gaussiana
def gauss(x, A, mu, sigma):
	return A*np.exp(-0.5*np.power(((x-mu)/sigma), 2))

# retorna dados da base
def get_data(path):
	return np.array(pd.read_csv(path, header=None))

# calcula fitting
def fitting(source):
	# definindo dados
	# placeholder pra source: "../centerprofile/neutral_sort/bs000_N_N_0.txt"
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
def classification():
	return None
	
# chamando funcao principal
if __name__ == "__main__":
	analizer()
