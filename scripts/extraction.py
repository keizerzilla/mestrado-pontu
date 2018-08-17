#!/usr/bin/python3
#!python3

import sys
import os
import subprocess
import parse
import time

# funcao de extracao de atributos
def extraction(param_ext, param_cut, param_cloud, param_out, perf_file=None):
	start = time.time()
	subprocess.call(["../bin/char-ext", param_ext, param_cut, param_cloud, param_out])
	elapsed = time.time() - start
	
	if perf_file != None:
		perf_file.write("{}\n".format(str(elapsed)))

# main code
def main():
	argc = len(sys.argv)
	
	extractors = ["hu", "zernike", "tchebychev", "legendre"]
	
	if argc < 5:
		print("Uso: <extractor> <cut> <input_cloud> <output_dir> <hard_mode>")
		print("<extrator>: uma das opções: " + str(extractors));
		print("<cut>: valor do corte da nuvem em mm")
		print("<input_cloud>: arquivo (ou pasta) com as nuvens")
		print("<output_dir>: pasta aonde os arquivos de resultado serao salvos")
		print("<hard_mode>: opcional - 1 para ativar")
		exit(1)
	
	parse_format = "bs{:d}_{}_{}_{:w}.cloud"
	
	extractor = sys.argv[1]
	cut = sys.argv[2]
	cloud = sys.argv[3]
	outfile = sys.argv[4] + extractor + ".ft"
	classfile = sys.argv[4] + extractor + ".cl"
	
	hard_mode = False
	if argc == 6:
		if sys.argv[5] == "1":
			hard_mode = True
	
	if os.path.isfile(cloud):
		extraction(extractor, cut, cloud, outfile)
	elif os.path.isdir(cloud):
		count = 0
		cf = open(classfile, "w")
		ftimes = open("../data/results/times/" + extractor + "_times.txt", "a")
		
		for filename in os.listdir(cloud):
			if filename.endswith(".cloud"):
				sample_class = str(parse.parse(parse_format, filename)[0])
				
				if hard_mode:
					sample_index = str(parse.parse(parse_format, filename)[3])
					if sample_index != "0":
						continue
					
				extraction(extractor, cut, cloud + filename, outfile, ftimes)
				cf.write(str(sample_class) + "\n")
				count += 1
		
		cf.close()
		ftimes.close()
		print(str(count) + " nuvens analisadas")
	else:
		print("ERRO DE DIRETORIO - ABORTANDO")
		exit(1)

# main script RUN!!!
main()
