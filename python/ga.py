"""-----------------------------------------------------------------------------
1. inicializa populacao
2. calcula fitness
3. realiza selecao dos mais aptos
4. computa crossover
5. mutação
6. volta para 2.

>> [[INCOMPLETO]] <<
-----------------------------------------------------------------------------"""

import random
import pandas as pd

class Chromosome():
	def __init__(self, moment, index, value, sample, subject):
		self.moment = moment
		self.index = index
		self.value = value
		self.sample = sample
		self.subject = subject

class Creature():
	def __init__(self):
		self.chromosomes = []
	
	def addChromosome(self, chromosome):
		self.chromosomes.append(chromosome)
	
	def getMoments(self):
		return [c.moment for c in self.chromosomes]
	
	def getIndexes(self):
		return [c.index for c in self.chromosomes]
		
	def getValues(self):
		return [c.value for c in self.chromosomes]

class Population():
	def __init__(self, datasets):
		self.chromosomes = []
		self.chromo_training = []
		self.chromo_testing = []
		
		
		for data in datasets:
			print("CARREGANDO: {} [{}]".format(data["name"], data["path"]))
			
			df = pd.read_csv(data["path"], header=None)
			cs = ["f"+str(x) for x in range(len(df.columns)-2)]
			cs = cs + ["sample", "subject"]
			df.columns = cs
			
			for i, row in df.iterrows():
				for j in range(len(df.columns)-2):
					c = Chromosome(data["name"], j, row[j], row["sample"], row["subject"])
					self.chromosomes.append(c)
		
		random.shuffle(self.chromosomes)
		print("TOTAL DE CROMOSSOMOS: {}".format(len(self.chromosomes)))
	
	
	
if __name__ ==  "__main__":
	legendre = {"name" : "legendre",
	            "path" : "../results/CLEAN_C80_D225_ICP/neutral-legendre.dat"}
	tchebychev = {"name" : "tchebychev",
	              "path" : "../results/CLEAN_C80_D225_ICP/neutral-tchebychev.dat"}
	datasets = [legendre, tchebychev]
	
	population = Population(datasets)
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
