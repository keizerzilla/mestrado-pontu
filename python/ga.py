"""-----------------------------------------------------------------------------
1. inicializa populacao
2. calcula fitness
3. realiza selecao dos mais aptos
4. computa crossover
5. mutação
6. volta para 2.
-----------------------------------------------------------------------------"""

fitscores = [1]

def init_population():
	pass

def fitness():
	pass

def selection():
	pass

def crossover():
	pass

def mutation():
	pass

if __name__ ==  "__main__":
	init_population()
	
	while max(fitscores) < 0.9:
		fitness()
		selection()
		crossover()
		mutation()
	
	print("Done!")
