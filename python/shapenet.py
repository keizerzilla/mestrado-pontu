import os

def obj2xyz(source, dest):
	"""Converte conjunto de nuvens .obj em formato .xyz usando pcl_obj2ply.

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

