import sys
import os
import subprocess
import parse

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
header -- O cabeçalho do arquivo de saída
"""
def moment_extraction_batch(moment, dataset, output, header):
	with open(output, "w") as dump:
		dump.write(header+"\n");
		for cloud in os.listdir(dataset):
			if cloud.endswith(".xyz"):
				dump.write(moment_extraction_cloud(moment, dataset+"/"+cloud))
				print("{} OK".format(cloud))

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
	datasets = ["../datasets/graphs"]
	moments = ["hututu"]
	header = "l1,l2,l3,l4,l5,l6,l7,l8,l9,l10,l11,l12,l13,l14,l15,l16,l17,l18,l19,l20,l21,sample,subject"
	#header = "l1,l2,l3,sample,subject"
	
	for data in datasets:
		for moment in moments:
			output = "../results/graphs/nosetip/{}-{}.dat".format(moment, os.path.split(data)[1])
			moment_extraction_batch(moment, data, output, header)
	
	#extract_graph("../datasets/bosphorus_no-outliers/Neutrals", "../datasets/graphs")
	
	
