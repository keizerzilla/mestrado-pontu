"""
feats.py
Artur Rodrigues Rocha Neto
2018

Funções para extração de momentos. Até o momento, o padrão de arquivo Bosphorus
é um único suportado. Mudanças futuras: FRGC e EURECOM.
"""

import os
import sys
import time
import parse
import subprocess

mcalc_exec = "../bin/mcalc"
parse_fmt = "bs{:d}_{:w}_{:w}_{:d}.xyz"

def moment_extraction_cloud(moment, cloud, cut):
	"""
	Executa extração de momentos em uma nuvem e parsing das informações de
	amostra e classe (padrão da base Bosphorus). Por enquanto apenas nuvem
	inteira, depois mudar para receber tipo de corte como argumento.

	:param moment: momento a ser usado
	:param cloud: nuvem alvo
	:param cut: tipo de corte
	:return: string com os momentos seguidos de amostra e classe
	"""
	
	path, filename = os.path.split(cloud)
	match = parse.parse(parse_fmt, filename)
	
	subject = str(match[0])
	tp = str(match[1])
	ex = str(match[2])
	sample = str(match[3])
	
	cmd = [mcalc_exec, "-m", moment, "-i", cloud, "-o", "stdout", "-c", cut]
	ans = subprocess.run(cmd, stdout=subprocess.PIPE).stdout
	ans = ans[:-2].decode("utf-8").replace(" ", ",")
	ans = ans + ",{},{}\n".format(sample, subject)
	
	return ans

def moment_extraction_batch(moment, cut, dataset, output):
	"""
	Executa extração de momentos em um conjunto de nuvens, além do parsingo de 
	informações de amostra e classe (padrão Bosphorus).

	:param moment: momento a ser usado
	:param cut: tipo de corte
	:param dataset: pasta com as nuvens
	:param output: arquivo de saída aonde os momentos serão salvos
	"""
	
	print("[{:<10}] - [ {} ] - [{}]".format(moment, cut, dataset))
	i = 0
	with open(output, "w") as dump:
		for cloud in os.listdir(dataset):
			if cloud.endswith(".xyz"):
				full_path = dataset + "/" + cloud
				ans = moment_extraction_cloud(moment, full_path, cut)
				dump.write(ans)
				i = i + 1
				print("\r{}".format(i), end="\r")
	print("[    OK    ] - [ {} nuvens analisadas ]".format(i))

