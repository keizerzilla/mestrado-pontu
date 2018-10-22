"""
feats.py
Artur Rodrigues Rocha Neto
2018

Funções para extração de momentos. Até o momento, o padrão de arquivo Bosphorus
é um único suportado.
"""

import os
import sys
import time
import parse
import subprocess

"""
Executa extração de momentos em uma nuvem e parsing das informações de amostra e 
classe (padrão da base Bosphorus).

moment -- O momento a ser usado
cloud -- A nuvem alva
slice_t -- O tipo de fatiamento

return -- um string com os momentos seguidos de amostra e classe (CSV)
"""
def moment_extraction_cloud(moment, cloud, slice_t):
	mcalc_exec = "../bin/mcalc"
	parse_fmt = "bs{:d}_{}_{}_{:w}.xyz"
	path, filename = os.path.split(cloud)
	
	cl = str(parse.parse(parse_fmt, filename)[0])
	ix = str(parse.parse(parse_fmt, filename)[3])
	
	cmd = [mcalc_exec, "-m", moment, "-c", cloud, "-o", "stdout", "-s", slice_t]
	ans = subprocess.run(cmd, check=True, stdout=subprocess.PIPE).stdout
	ans = ans.decode("utf-8")[:-2].replace(" ", ",") + ",{},{}\n".format(ix, cl)
	
	return ans

"""
Executa extração de momentos em um conjunto de nuvens, além do parsingo de 
informações de amostra e classe (padrão Bosphorus).

moment -- O momento a ser usado
dataset -- A pasta com as nuvens
slice_t -- O tipo de fatiamento
output -- O arquivo de saída aonde os momentos serão salvos
"""
def moment_extraction_batch(moment, dataset, slice_t, output):
	with open(output, "w") as dump:
		for cloud in os.listdir(dataset):
			if cloud.endswith(".xyz"):
				full_path = dataset + "/" + cloud
				ans = moment_extraction_cloud(moment, full_path, slice_t)
				dump.write(ans)
				print("[{}] >> {}: OK".format(cloud, moment))

if __name__ == "__main__":
	scenarios = ["bosphorus",
	             "bosphorus-outlier",
	             "bosphorus-outlier-densit200",
	             "bosphorus-outlier-densit200-crop60",
	             "bosphorus-outlier-densit200-crop70",
	             "bosphorus-outlier-densit200-crop80",
	             "bosphorus-outlier-densit200-crop60-icp",
	             "bosphorus-outlier-densit200-crop70-icp",
	             "bosphorus-outlier-densit200-crop80-icp",
	             "bosphorus-outlier-densit225",
	             "bosphorus-outlier-densit225-crop60",
	             "bosphorus-outlier-densit225-crop70",
	             "bosphorus-outlier-densit225-crop80",
	             "bosphorus-outlier-densit225-crop60-icp",
	             "bosphorus-outlier-densit225-crop70-icp",
	             "bosphorus-outlier-densit225-crop80-icp"]
	
	moments = ["zernike"]
	datasets = ["../datasets/" + x + "/neutral" for x in scenarios]
	
	for data in datasets:
		scenario = data.split("/")[2]
		folder = "../results/{}".format(scenario)
		
		try:
			os.mkdir(folder)
		except:
			print("OPS! Diretorio jah existe!")
		
		for moment in moments:
			out = folder + "/{}-{}.dat".format(os.path.split(data)[1], moment)
			moment_extraction_batch(moment, data, "w", out)
	
