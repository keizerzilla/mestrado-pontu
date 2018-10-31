"""
combine.py
Artur Rodrigues Rocha Neto
2018

Script gerador de vetores de característica otimizados através da combinação
de diferentes configurações (cortes, fatiamentos, momentos, etc.)
"""

from dataset import *
from features import *
from classification import *



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
		self.X_train = []
		self.y_train = []
		self.X_test = []
		self.y_test = []
		self.chromosomes = []
		
		for data in datasets:
			print("CARREGANDO: [{}]".format(data["name"]))
			
			df = pd.read_csv(data["path"], header=None)
			cols = [data["name"]+str(x) for x in range(len(df.columns)-2)]
			cols = cols + ["sample", "class"]
			df.columns = cols
			
			trainset = df.loc[df["sample"] == 0].drop(["sample"], axis=1)
			X_train = trainset.drop(["class"], axis=1)
			y_train = trainset[["class"]]
			
			testset = df.loc[df["sample"] != 0].drop(["sample"], axis=1)
			X_test = testset.drop(["class"], axis=1)
			y_test = testset[["class"]]
			
			self.X_train.append(X_train)
			self.y_train = y_train
			self.X_test.append(X_test)
			self.y_test = y_test
		
		self.X_train = pd.concat(self.X_train, axis=1)
		self.X_test = pd.concat(self.X_test, axis=1)
		
		scaler = StandardScaler().fit(self.X_train)
		self.X_train = scaler.transform(self.X_train)
		self.X_test = scaler.transform(self.X_test)
		
		self.y_train = np.ravel(self.y_train)
		self.y_test = np.ravel(self.y_test)

