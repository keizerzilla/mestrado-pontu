import os
import time
import subprocess
import pandas as pd

moments = ["sm", "zo", "ze", "zm", "zf"]
dataset = "../datasets/bosphorus_neutral/"
bindir = "../pontu/bin/bosphorus"

dump = {
	"sm" : [],
	"zo" : [],
	"ze" : [],
	"zm" : [],
	"zf" : [],
}

print("iniciando contagem de tempo (custo computacional)...")

for i in range(100):
	for cloud in os.listdir(dataset):
		fullPath = dataset + "/" + cloud
		for moment in moments:
			cmd = [bindir, fullPath, moment]
			
			start = time.time()
			ans = subprocess.run(cmd, stdout=subprocess.PIPE).stdout
			stop = time.time()
			delta = stop - start
			
			dump[moment].append(delta)
	
	print("iteracao {} ok!".format(i+1))

print(len(dump["sm"]))
print(len(dump["zo"]))
print(len(dump["ze"]))
print(len(dump["zm"]))
print(len(dump["zf"]))

dump = pd.DataFrame(dump)
dump.to_csv("../results/shapenet/count_time.csv", index=None)
print("pronto!\n")

