"""
features.py
Artur Rodrigues Rocha Neto
2018

Extração de características (momentos). Padrão de arquivos Bosphorus é o único
suportado. Mudanças futuras: padrões FRGC v2.0 e EURECOM.
"""

import os
import time
import parse
import subprocess

mcalc = "../bin/mcalc"
fmt = "bs{:d}_{:w}_{:w}_{:d}.xyz"
	
def cloud_extraction(moment, cloud, cut):
	"""
	Chama programa extrator, passando momento e corte, e retorna vetor de
	atributos completo (incluíndo amostra, classe, tipo e expressão).
	
	:param moment: O momento a ser calculado
	:param cloud: A nuvem alvo de onde será extráido moment
	:param cut: O corte (fatiamento) a ser usado
	"""
	path, filename = os.path.split(cloud)
	match = parse.parse(fmt, filename)
	
	if match == None:
		return None
	
	subject = str(match[0])
	tp = str(match[1])
	ex = str(match[2])
	sample = str(match[3])
	
	cmd = [mcalc, "-m", moment, "-i", cloud, "-o", "stdout", "-c", cut]
	ans = subprocess.run(cmd, stdout=subprocess.PIPE).stdout
	#ans = ans[:-2].decode("utf-8").replace(" ", ",")
	ans = ans[:-1].decode("utf-8").replace(" ", ",")
	ans = ans + ",{},{},{},{}\n".format(sample, subject, tp, ex)
	
	return ans

def batch_extraction(moment, dataset, cut, output):
	"""
	Extrái momentos repetidamente ao longo de uma pasta com várias nuvens.
	
	:param moment: O momento a ser calculado
	:param cloud: A nuvem alvo de onde será extráido moment
	:param cut: O corte (fatiamento) a ser usado
	:param output: Arquivo aonde ficarão salvos os momentos das diversas nuvens
	"""
	print("[{:<12}] - [ {} ] - [{}]".format(moment, cut, dataset))
	
	count = 0
	start_time = time.time()
	
	with open(output, "w") as dump:
		for cloud in os.listdir(dataset):
			if cloud.endswith(".xyz"):
				fullPath = dataset + "/" + cloud
				ans = cloud_extraction(moment, fullPath, cut)
				
				if ans == None:
					continue
				
				dump.write(ans)
				count = count + 1
				print("\r{}".format(count), end="\r")
	
	elapsed = time.time() - start_time
	velocity = round(elapsed / count, 6)
	
	print("[     OK     ] - [ {} nuvens, {} seg/nuvem]".format(count, velocity))

def total_extraction(faces, scenarios, cuts, moments, rdir="results"):
	"""
	Extrái diversos cenários de experimentação.
	
	:param faces: Tipos de faces que se quer analisar
	:param scenarios: Pastas com os diversos cenários de nuvens
	:param cuts: Os corte (fatiamentos)
	:param moments: Os momentos a serem extraídos
	:param rdir: Aonde se deve salvar os resultados de extração (pasta)
	"""
	for face in faces:
		sets = ["../datasets/" + x + "/{}".format(face) for x in scenarios]
		for cut, cut_folder in cuts.items():
			for data in sets:
				scenario = data.split("/")[2]
				folder = "../{}/{}/{}/".format(rdir, cut_folder, scenario)
				os.makedirs(folder, exist_ok=True)
				for moment in moments:
					out = folder + "{}-{}.dat".format(face, moment)
					batch_extraction(moment, data, cut, out)

