#!/usr/bin/python3
#!python3

import sys
import os
import xml.etree.ElementTree as ET

# funcao que converte de formato opencvxml para formato tutucloud
def convert_xml_to_cloud(filename, outputdir):
	try:
		tree = ET.parse(filename)
	except FileNotFoundError:
		print("ERROR: FILE NOT FOUND!")
		exit(2)

	root = tree.getroot()
	data = root.find("Face").find("data").text.split()

	fileout = os.path.basename(filename).replace(".xml", ".cloud")
	fileout = os.path.join(outputdir, fileout)
	cloud = open(fileout, "w")

	for i in range(0, len(data), 3):
		line = str(data[i]) + " " + str(data[i+1]) + " " + str(data[i+2]) + "\n"
		cloud.write(line)

	cloud.close()

# main code
def main():
	argc = len(sys.argv)
	
	print("convcloud.py (Artur Rodrigues)")
	
	if argc != 3:
		print("Uso: <path_input> <path_output>")
		print("<path_input>: arquivo (ou pasta) com as nuvens")
		print("<path_output>: pasta aonde as nuvens convertidas ficarão salvas")
		exit(1)

	path1 = sys.argv[1]
	path2 = sys.argv[2]

	# checa se diretorio de saida jah existe
	# se nao existir, cria
	if not os.path.exists(path2):
		os.makedirs(path2)

	if os.path.isfile(path1):
		# chama convert_xml_to_cloud soh nesse arquivo
		convert_xml_to_cloud(path1, path2)
		print("Arquivo [" + path1 + "] convertido com sucesso")
		print("1 arquivo(s) convertido(s)")
	elif os.path.isdir(path1):
		# chama convert_xml_to_cloud em todos os xml do folder
		count = 0
		for filename in os.listdir(path1):
			if filename.endswith(".xml"):
				print("Convertendo [" + filename + "]...")
				convert_xml_to_cloud(os.path.join(path1, filename), path2)
				print("Arquivo [" + filename + "] convertido com sucesso")
				count += 1
		print(str(count) + " arquivo(s) convertido(s)")
	else:
		# erro!!!
		print("I'M ERROR")
		exit()

# script
main()
