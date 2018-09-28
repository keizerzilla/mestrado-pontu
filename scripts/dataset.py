import os
import sys
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
		print(outf + " OK")

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

if __name__ == "__main__":
	source = "/home/artur/github/latin/datasets/dump"
	dest = "/home/artur/github/latin/datasets/dump"
	pcd2xyz(source, dest)
