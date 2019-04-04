"""
dataset.py
Artur Rodrigues Rocha Neto
2018

Funções de conversão/normalização dos arquivos das bases de dados.
"""

import os
import sys
import parse
import shutil
import subprocess
import numpy as np
import pandas as pd

def isInt(s):
	try: 
		int(s)
		return True
	except ValueError:
		return False

def cloud2xyz(source):
	"""
	Normaliza as extenções dos arquivos de nuvem (FUNÇÃO HISTÓRICA).

	:param source: pasta com os arquivos a serem convertidos
	"""
	
	for filename in os.listdir(source):
		inpf = os.path.join(source, filename)
		
		if not os.path.isfile(inpf) or not inpf.endswith(".cloud"):
			continue
		
		newname = inpf.replace(".cloud", ".xyz")
		output = os.rename(inpf, newname)
		print(filename + " OK")

def xyz2pcd(source, dest):
	"""
	Converte conjunto de nuvens .xyz em formato .pcd usando pcl_xyz2pcd

	:param source: pasta com os arquivos a serem convertidos
	:param dest: pasta destino das nuvens convertidas
	"""
	
	for cloud in os.listdir(source):
		inpf = os.path.join(source, cloud)
		
		if not os.path.isfile(inpf) or not inpf.endswith(".xyz"):
			continue
		
		outf = os.path.join(dest, cloud.replace(".xyz", ".pcd"))
		cmd = ["pcl_xyz2pcd", inpf, outf]
		subprocess.call(cmd)
		print(outf + " OK")

def pcd2xyz(source, dest):
	"""
	Converte conjunto de nuvens .pcd em formato .xyz usando pcl_pcd2ply

	:param source: pasta com os arquivos a serem convertidos
	:param dest: pasta destino das nuvens convertidas
	"""
	
	for cloud in os.listdir(source):
		inpf = os.path.join(source, cloud)
		
		if not os.path.isfile(inpf) or not inpf.endswith(".pcd"):
			continue
		
		outf = os.path.join(dest, cloud.replace(".pcd", ".ply"))
		cmd = ["pcl_pcd2ply", "-format", "0", "-use_camera", "0", inpf, outf]
		print(" ".join(cmd))
		subprocess.call(cmd)
		
		data = ""
		with open(outf, "r") as ply:
			data = ply.read()
			data = data[data.find("end_header")+11:]
		
		with open(outf, "w") as dump:
			dump.write(data)
		
		os.rename(outf, outf.replace(".ply", ".xyz"))
		
		print(outf + " OK")

def obj2xyz(source, dest):
	"""
	Converte conjunto de nuvens .obj em formato .xyz usando pcl_obj2ply

	:param source: pasta com os arquivos a serem convertidos
	:param dest: pasta destino das nuvens convertidas
	"""
	
	for cloud in os.listdir(source):
		inpf = os.path.join(source, cloud)
		
		if not os.path.isfile(inpf) or not inpf.endswith(".obj"):
			continue
		
		outf = os.path.join(dest, cloud.replace(".obj", ".ply"))
		cmd = ["pcl_obj2ply", "-format", "1", "-use_camera", "0", inpf, outf]
		print(" ".join(cmd))
		subprocess.call(cmd)
		
		data = ""
		with open(outf, "r") as ply:
			data = ply.read()
			data = data[data.find("end_header")+11:]
		
		with open(outf, "w") as dump:
			dump.write(data)
		
		os.rename(outf, outf.replace(".ply", ".xyz"))
		
		print(outf + " OK")

def bosphorus_filenames(folder, output):
	"""
	Pega nome dos arquivos de uma pasta para melhor separar conjuntos neutral e
	nonneutral no futuro

	:param folder: pasta com as nuvens
	:param output: arquivo com os nomes das nuvens
	"""
	
	with open(output, "w") as dump:
		for cloud in os.listdir(folder):
			inpf = os.path.join(folder, cloud)
			
			if not os.path.isfile(inpf):
				continue
			
			dump.write(os.path.splitext(cloud)[0] + "\n")

def bosphorus_group(source, dest):
	"""
	Agrupa todas as nuvens de um diretório divido por indivíduo em uma pasta só.

	:param source: pasta com nuvens divididas por indivíduo
	:param dest: pasta com todas as nuvens juntas
	"""
	
	for path in os.listdir(source):
		folder = os.path.join(source, path)
		if os.path.isdir(folder):
			for cloud in os.listdir(folder):
				srcf = os.path.join(folder, cloud)
				dstf = os.path.join(dest, cloud)
				shutil.move(srcf, dstf)
				print("{} OK".format(dstf))

def bosphorus_split(folder, dirneutral, dirnonneutral, dirother):
	"""
	Separa os conjuntos neutral, nonneutral e other de um base de arquivos no
	formato bosphorus. Os arquivos contendo os nomes dos arquivos devem estar
	numa pasta chamada 'res/'.

	:param folder: pasta com o conjunto completo
	:param dirneutral: pasta que ficarão salvas as nuvens neutras
	:param dirnonneutral: pasta que ficarão salvas as nuvens não-neutras
	:param dirother: pasta que ficarão salvas as demais nuvens
	"""
	
	neutrals = []
	nonneutrals = []
	
	with open("res/neutralfiles.txt", "r") as content_neutral:
		neutrals = content_neutral.read().split()
	with open("res/nonneutralfiles.txt", "r") as content_nonneutral:
		nonneutrals = content_nonneutral.read().split()
	
	for cloud in os.listdir(folder):
		inpf = os.path.join(folder, cloud)
		
		if not os.path.isfile(inpf):
			continue
		
		temp = os.path.splitext(cloud)[0]
		if temp in neutrals:
			shutil.move(inpf, os.path.join(dirneutral, cloud))
		elif temp in nonneutrals:
			shutil.move(inpf, os.path.join(dirnonneutral, cloud))
		else:
			shutil.move(inpf, os.path.join(dirother, cloud))

def bosphorus_setup(folder):
	"""
	A única função que você precisará na sua vida! Agrupa, divide e converte as
	nuvens de um diretório em formato bosphorus, criando o ambiente completo
	para testes.

	:param folder: pasta com o conjunto completo
	"""
	
	try:
		os.mkdir(folder+"/all")
		os.mkdir(folder+"/neutral")
		os.mkdir(folder+"/nonneutral")
		os.mkdir(folder+"/other")
	except:
		pass
	
	bosphorus_group(folder, folder+"/all")
	bosphorus_split(folder+"/all",
	                folder+"/neutral",
	                folder+"/nonneutral",
	                folder+"/other")
	pcd2xyz(folder+"/neutral", folder+"/neutral")
	pcd2xyz(folder+"/nonneutral", folder+"/nonneutral")

def eurecom_setup(folder):
	os.makedirs(folder + "/all", exist_ok=True)
	os.makedirs(folder + "/neutral", exist_ok=True)
	os.makedirs(folder + "/nonneutral", exist_ok=True)
	os.makedirs(folder + "/other", exist_ok=True)
	
	fmt = "depth_{:d}_s{:d}_{:w}.xyz"
	
	for d in os.listdir(folder):
		if isInt(d):
			subject = int(d)
			s1 = folder + "/" + d + "/s1/3DObj"
			s2 = folder + "/" + d + "/s2/3DObj"
			
			for d1 in os.listdir(s1):
				srcf = os.path.join(s1, d1)
				dstf = os.path.join(folder + "/all", d1)
				shutil.move(srcf, dstf)
			
			for d2 in os.listdir(s2):
				srcf = os.path.join(s2, d2)
				dstf = os.path.join(folder + "/all", d2)
				shutil.move(srcf, dstf)
	
	#obj2xyz(folder+"/all", folder+"/all") // ajeitar a quarta coluna bizarra
	
	for f in os.listdir(folder + "/all"):
		match = parse.parse(fmt, f)
		subject = str(match[0])
		sample = int(match[1]-1)
		tp = str(match[2])
		
		bs_type = "N_N"
		if tp == "Neutral":
			bs_type = "N_N"
		elif tp == "Smile":
			bs_type = "E_SMILE"
		elif "Occlusion" in tp:
			bs_type = "O_" + tp.replace("Occlusion", "").upper()
		elif tp == "LightOn":
			bs_type = "N_N"
			sample = sample + 1
		elif tp == "LeftProfile":
			bs_type = "YR_L90"
		elif tp == "RightProfile":
			bs_type = "YR_R90"
		elif tp == "OpenMouth":
			bs_type = "LFAU_MOUTH"
		
		new_name = "bs{:03d}_{}_{}.xyz".format(int(subject), bs_type, sample)
		
		print(f, " =>> ", new_name)
		
def translate_to_zero(in_folder, out_folder):
	origin = np.array([0, 0, 0])
	for f in os.listdir(in_folder):
		if ".xyz" in f:
			cloud = in_folder + f
			df = pd.read_csv(cloud, sep=' ', header=None)
			pcd_data = np.array(df)
			centroid = np.mean(pcd_data, axis=0)
			direction = origin - centroid
			pcd_trans = pcd_data + direction
			df = pd.DataFrame(pcd_trans)
			df.to_csv(out_folder + f, header=None, sep=' ', index=None)
			
			print(f, " done!")

