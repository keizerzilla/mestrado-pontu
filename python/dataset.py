import os
import sys
import shutil
import subprocess

"""
dataset.py
Artur Rodrigues Rocha Neto
2018

Funções de conversão/normalização dos arquivos das bases de dados.
"""

"""
Normaliza as extenções dos arquivos de nuvem (FUNÇÃO HISTÓRICA).

source -- A pasta com os arquivos a serem convertidos
"""
def cloud2xyz(source):
	for filename in os.listdir(source):
		inpf = os.path.join(source, filename)
		
		if not os.path.isfile(inpf) or not inpf.endswith(".cloud"):
			continue
		
		newname = inpf.replace(".cloud", ".xyz")
		output = os.rename(inpf, newname)
		print(filename + " OK")

"""
Converte conjunto de nuvens .xyz em formato .pcd usando pcl_xyz2pcd

source -- A pasta com os arquivos a serem convertidos
dest -- Pasta destino das nuvens convertidas
"""
def xyz2pcd(source, dest):
	for cloud in os.listdir(source):
		inpf = os.path.join(source, cloud)
		
		if not os.path.isfile(inpf) or not inpf.endswith(".xyz"):
			continue
		
		outf = os.path.join(dest, cloud.replace(".xyz", ".pcd"))
		cmd = ["pcl_xyz2pcd", inpf, outf]
		subprocess.call(cmd)
		print(outf + " OK")

"""
Converte conjunto de nuvens .pcd em formato .xyz usando pcl_pcd2ply + Python

source -- A pasta com os arquivos a serem convertidos
dest -- Pasta destino das nuvens convertidas
"""
def pcd2xyz(source, dest):
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

"""
Converte conjunto de nuvens .obj em formato .xyz usando pcl_obj2ply + Python

source -- A pasta com os arquivos a serem convertidos
dest -- Pasta destino das nuvens convertidas
"""
def obj2xyz(source, dest):
	for cloud in os.listdir(source):
		inpf = os.path.join(source, cloud)
		
		if not os.path.isfile(inpf) or not inpf.endswith(".obj"):
			continue
		
		outf = os.path.join(dest, cloud.replace(".obj", ".ply"))
		cmd = ["pcl_obj2ply", "-format", "0", "-use_camera", "0", inpf, outf]
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

"""
Pega nome dos arquivos de uma pasta para melhor separar conjuntos neutral e
nonneutral no futuro

folder -- Pasta com as nuvens
output -- arquivo com os nomes das nuvens
"""
def bosphorus_filenames(folder, output):
	with open(output, "w") as dump:
		for cloud in os.listdir(folder):
			inpf = os.path.join(folder, cloud)
			
			if not os.path.isfile(inpf):
				continue
			
			dump.write(os.path.splitext(cloud)[0] + "\n")

"""
Agrupa todas as nuvens de um diretório divido por indivíduo em uma pasta só.

source -- Pasta com nuvens divididas por indivíduo
dest -- Pasta com todas as nuvens juntas
"""
def bosphorus_group(source, dest):
	for path in os.listdir(source):
		folder = os.path.join(source, path)
		if os.path.isdir(folder):
			for cloud in os.listdir(folder):
				srcf = os.path.join(folder, cloud)
				dstf = os.path.join(dest, cloud)
				shutil.copyfile(srcf, dstf)

"""
Separa os conjuntos neutral, nonneutral e other de um base de arquivos no
formato bosphorus. Os arquivos contendo os nomes dos arquivos devem estar numa
pasta chamada 'res/'. Falta testar!

folder -- Pasta com o conjunto completo
dirneutral -- Pasta que ficarão salvas as nuvens neutras
dirnonneutral -- Pasta que ficarão salvas as nuvens não-neutras
dirother -- Pasta que ficarão salvas as nuvens nem neutras nem não-neutras
"""
def bosphorus_split(folder, dirneutral, dirnonneutral, dirother):
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
			shutil.copyfile(inpf, os.path.join(dirneutral, cloud))
		elif temp in nonneutrals:
			shutil.copyfile(inpf, os.path.join(dirnonneutral, cloud))
		else:
			shutil.copyfile(inpf, os.path.join(dirother, cloud))

if __name__ == "__main__":
	bosphorus_group("../datasets/01_bosphorus", "../datasets/01_bosphorus/all")
	
