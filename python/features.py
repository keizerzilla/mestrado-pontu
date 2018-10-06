import os
import sys
import time
import parse
import subprocess

"""
feats.py
Artur Rodrigues Rocha Neto
2018

Funções para extração de momentos. Até o momento, o padrão de arquivo Bosphorus
é um único suportado.
"""

"""
Executa extração de momentos em uma nuvem e parsing das informações de amostra e 
classe (padrão Bosphorus).

moment -- O momento a ser usado
cloud -- A nuvem alva

return -- um string com os momentos seguidos de amostra e classe (CSV)
"""
def moment_extraction_cloud(moment, cloud):
	parse_fmt = "bs{:d}_{}_{}_{:w}.xyz"
	path, filename = os.path.split(cloud)
	
	cl = str(parse.parse(parse_fmt, filename)[0])
	ix = str(parse.parse(parse_fmt, filename)[3])
	
	cmd = ["../bin/mcalc", "-m", moment, "-c", cloud, "-o", "stdout"]
	
	ret = subprocess.run(cmd, check=True, stdout=subprocess.PIPE).stdout
	ret = ret.decode("utf-8")[:-2].replace(" ", ",") + ",{},{}\n".format(ix, cl)
	
	return ret

"""
Executa extração de momentos em um conjunto de nuvens, além do parsingo de 
informações de amostra e classe (padrão Bosphorus).

moment -- O momento a ser usado
dataset -- A pasta com as nuvens
output -- O arquivo de saída aonde os momentos serão salvos
"""
def moment_extraction_batch(moment, dataset, output):
	with open(output, "w") as dump:
		for cloud in os.listdir(dataset):
			if cloud.endswith(".xyz"):
				dump.write(moment_extraction_cloud(moment, dataset+"/"+cloud))
				print("[{}] >> {}: OK".format(cloud, moment))

"""
Retira grafo de subamostragem bem louco.

dataset -- O conjunto de dados alvo
output -- Aonde as subnuvens ficarão salvas
"""
def extract_graph(dataset, output):
	for cloud in os.listdir(dataset):
		if cloud.endswith(".xyz"):
			infile = os.path.join(dataset, cloud)
			if not os.path.isfile(infile): continue
			outfile = os.path.join(output, cloud)
			cmd = ["../bin/cloudgraph", infile, outfile]
			subprocess.call(cmd)
			print(outfile + " OK")

if __name__ == "__main__":
	scenarios = ["bosphorus-outlier-densit200-crop60-icp",
	             "bosphorus-outlier-densit200-crop70-icp",
	             "bosphorus-outlier-densit200-crop80-icp",
	             "bosphorus-outlier-densit225-crop60-icp",
	             "bosphorus-outlier-densit225-crop70-icp",
	             "bosphorus-outlier-densit225-crop80-icp",
	             "bosphorus-outlier-densit200-nose",
	             "bosphorus-outlier-densit225-nose"]
	
	moments = ["hu1980", "hututu", "legendre", "tchebychev", "zernike"]
	datasets = ["../datasets/" + x + "/neutral" for x in scenarios]
	datasets += ["../datasets/" + x + "/nonneutral" for x in scenarios]
	
	for data in datasets:
		scenario = data.split("/")[2]
		folder = "../results/{}".format(scenario)
		
		try:
			os.mkdir(folder)
		except:
			pass
		
		for moment in moments:
			out = folder + "/{}-{}.dat".format(os.path.split(data)[1], moment)
			moment_extraction_batch(moment, data, out)
	
