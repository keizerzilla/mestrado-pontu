"""
combination.py
Artur Rodrigues Rocha Neto
2018

Script gerador de vetores de característica otimizados através da combinação
de diferentes configurações (cortes, fatiamentos, momentos, etc.)
"""

from classification import *
from pathlib import Path

def combination(dataset, n=2):
	"""
	Executa classificações exaustivas para uma lista de combinações de momentos.
	
	:param dataset: conjunto de dados base a ser testado
	:param n: número de elementos por combinação
	"""
	
	basepath = "../results/"
	slices = ["frontal/", "radial/", "sagittal/", "transversal/", "whole/"]
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
		result = "{:<85}{:<15}{:<7}".format(setting, classifier, rate)
		row = {"setting" : setting, "classifier" : classifier, "rate" : rate}
		df = df.append(row, ignore_index=True)
		print(result)
	
	res_path = "../results/combination{}/".format(n)
	try:
		Path(res_path).mkdir(parents=True, exist_ok=True)
	except:
		print("OPS! Diretorio jah existe!")
	
	df.to_csv(res_path + "{}.csv".format(dataset), index=False)

