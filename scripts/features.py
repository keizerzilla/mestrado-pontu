import sys
import os
import subprocess
import parse

"""
feats.py
Artur Rodrigues Rocha Neto
2018

Funções para extração de momentos e formatação de datasets. Até o momento, o
padrão de arquivo Bosphorus é um único suportado.
"""

"""
Normaliza as extenções dos arquivos de nuvem (HISTÓRICA).

folder -- A pasta aonde os arquivos estão salvas
"""
def normalize_extensions(folder):
	for filename in os.listdir(folder):
		infilename = os.path.join(folder, filename)
		if not os.path.isfile(infilename): continue
		oldbase = os.path.splitext(filename)
		newname = infilename.replace(".cloud", ".xyz")
		output = os.rename(infilename, newname)

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


if __name__ == "__main__":
	datasets = ["../datasets/bosphorus_no-outliers/Neutrals", "../datasets/bosphorus_no-outliers/NonNeutrals"]
	moments = ["hututu", "hu1980"]
	header = "j1,j2,j3,sample,subject" # ATENCAO PRA ESSE CARINHA AQUI!
	
	for data in datasets:
		for moment in moments:
			output = "../results/bosphorus_no-outliers/{}-{}.dat".format(moment, os.path.split(data)[1])
			moment_extraction_batch(moment, data, output, header)
	
	
