"""
features.py
Artur Rodrigues Rocha Neto
2018

Extração de características (momentos). Padrão de arquivos Bosphorus é o único
suportado. Mudanças futuras: padrões FRGC v2.0 e EURECOM.
"""

import os
import parse
import subprocess

class MomentExtractor():
	def __init__(self, mcalc="../bin/mcalc", fmt="bs{:d}_{:w}_{:w}_{:d}.xyz"):
		self.mcalc = mcalc
		self.fmt = fmt
	
	def cloudExtraction(self, moment, cloud, cut):
		path, filename = os.path.split(cloud)
		match = parse.parse(self.fmt, filename)
		
		subject = str(match[0])
		tp = str(match[1])
		ex = str(match[2])
		sample = str(match[3])
		
		cmd = [self.mcalc, "-m", moment, "-i", cloud, "-o", "stdout", "-c", cut]
		ans = subprocess.run(cmd, stdout=subprocess.PIPE).stdout
		ans = ans[:-2].decode("utf-8").replace(" ", ",")
		ans = ans + ",{},{},{},{}\n".format(sample, subject, tp, ex)
		
		return ans
	
	def batchExtraction(self, moment, dataset, cut, output):
		print("[{:<12}] - [ {} ] - [{}]".format(moment, cut, dataset))
		
		count = 0
		with open(output, "w") as dump:
			for cloud in os.listdir(dataset):
				if cloud.endswith(".xyz"):
					fullPath = dataset + "/" + cloud
					ans = self.cloudExtraction(moment, fullPath, cut)
					dump.write(ans)
					count = count + 1
					print("\r{}".format(count), end="\r")
		
		print("[     OK     ] - [ {} nuvens analisadas ]".format(count))
	
	def totalExtraction(self, faces, scenarios, cuts, moments, rdir="results"):
		for face in faces:
			sets = ["../datasets/" + x + "/{}".format(face) for x in scenarios]
			for cut, cut_folder in cuts.items():
				for data in sets:
					scenario = data.split("/")[2]
					folder = "../{}/{}/{}/".format(rdir, cut_folder, scenario)
					os.makedirs(folder, exist_ok=True)
					for moment in moments:
						out = folder + "{}-{}.dat".format(face, moment)
						self.batchExtraction(moment, data, cut, out)
		
