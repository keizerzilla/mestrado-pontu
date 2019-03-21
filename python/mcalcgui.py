"""
mcalcgui.py
Artur Rodrigues Rocha Neto
2019
Em construção.
"""

from tkinter import *
from tkinter import ttk
from tkinter.filedialog import askdirectory
from tkinter.scrolledtext import ScrolledText

class mcalcGUI():
	def __init__(self):
		# janela principal
		self.window = Tk()
		self.window.title("mcalc")
		self.window.resizable(False, False)
		
		# menu principal
		self.menu = Menu(self.window)
		self.menu.add_command(label="Arquivo")
		self.menu.add_command(label="Preferências")
		self.menu.add_command(label="Ferramentas")
		self.menu.add_command(label="Ajuda")
		self.window.config(menu=self.menu)
		
		# notebook de abas
		self.notebook = ttk.Notebook(self.window)
		
		self.cuts = {"whole"       : IntVar(),
					 "frontal"     : IntVar(),
					 "sagittal"    : IntVar(),
					 "transversal" : IntVar(),
					 "radial"      : IntVar(),
					 "upper"       : IntVar(),
					 "lower"       : IntVar()}

		self.moments = {"spheric"   : IntVar(),
				   		"hututu"    : IntVar(),
				   		"hu1980"    : IntVar(),
				   		"husiq"     : IntVar(),
				   		"zernike"   : IntVar(),
				   		"legendre"  : IntVar(),
				   		"chebyshev" : IntVar()}
		
		# aba de extracao
		self.tabExtraction = ttk.Frame(self.notebook)
		
		self.groupCuts = LabelFrame(self.tabExtraction, text="Cortes")
		for key, value in self.cuts.items():
			cb = Checkbutton(self.groupCuts, text=key, variable=value)
			cb.pack(anchor="w")
		self.groupCuts.pack(side=LEFT)
		
		self.groupMoments = LabelFrame(self.tabExtraction, text="Momentos")
		for key, value in self.moments.items():
			cb = Checkbutton(self.groupMoments, text=key, variable=value)
			cb.pack(anchor="w")
		self.groupMoments.pack(side=LEFT)
		
		self.groupScenarios = LabelFrame(self.tabExtraction, text="Cenários")
		
		self.chooseScenarios = Button(self.groupScenarios, text="Escolher",
		                       command=self.selectScenarios)
		self.chooseScenarios.pack()
		
		self.groupScenarios.pack(side=LEFT)
		
		self.extractButton = Button(self.tabExtraction, text="Extrair",
		                            command=self.extract)
		self.extractButton.pack(side=BOTTOM)
		
		self.notebook.add(self.tabExtraction, text="Extração")
		
		# aba de classificacao
		self.tabClassification = ttk.Frame(self.notebook)
		self.notebook.add(self.tabClassification, text="Classificação")
		
		# aba de combinacao
		self.tabCombination = ttk.Frame(self.notebook)
		self.notebook.add(self.tabCombination, text="Combinação")
		
		# fechando notebook
		self.notebook.pack(expand=1, fill="both")
		
		# console de saida
		self.console = ScrolledText(self.window, height=8, width=80)
		self.console.config(state="disabled")
		self.console.pack()
	
	def selectScenarios(self):
		ans = askdirectory(title="Bases para extração",
		                   initialdir="/home/artur/github/latin/datasets/")
		print(ans)
	
	def extract(self):
		cuts = [k for k, v in self.cuts.items() if v.get()]
		moments = [k for k, v in self.moments.items() if v.get()]
		print(cuts, moments)
		
	def run(self):
		self.window.mainloop()


if __name__ == "__main__":
	mcalc = mcalcGUI()
	mcalc.run()

