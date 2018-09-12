import os
import sys
import subprocess

"""
Normaliza as extenções dos arquivos de nuvem (HISTÓRICA).

folder -- A pasta aonde os arquivos estão salvas
"""
def cloud2xyz(folder):
	for filename in os.listdir(folder):
		infilename = os.path.join(folder, filename)
		if not os.path.isfile(infilename): continue
		newname = infilename.replace(".cloud", ".xyz")
		output = os.rename(infilename, newname)

"""
Converte conjunto de nuvens xyz em formato PCD usando pcl_xyz2pcd

folder -- A pasta com as nuvens originais
dest -- A pasta aonde as nuvens convertidas ficarão salvas
"""
def xyz2pcd(folder, dest):
	for filename in os.listdir(folder):
		infilename = os.path.join(folder, filename)
		if not os.path.isfile(infilename): continue
		outfile = dest + filename.replace(".xyz", ".pcd")
		cmd = ["pcl_xyz2pcd", infilename, outfile]
		subprocess.call(cmd)
		print(outfile + " OK")

if __name__ == "__main__":
	folder = "/home/artur/github/latin/datasets/bosphorus_no-outliers/NonNeutrals/"
	dest = "/home/artur/github/latin/datasets/bosphorus_pcd/NonNeutrals/"
	xyz2pcd(folder, dest)
