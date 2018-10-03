import pandas as pd
import matplotlib.pyplot as plt
import seaborn

"""
Análise monovariada global dos momentos
- plotar histogramas
- calcular média, desvio padrão e assimetria
- super incompleta, mas já aponta resultados interessantes
"""

# setando estilo e outras configs
seaborn.set()

# paths e arquivos
dataset = "../results/CLEAN_C80_D225_ICP/neutral-zernike.dat"
figpath = "res/"
result_file = "res/monovar.dat"
heatmap_file = "res/correlation_heatmap.png"
pairplot_file = "res/scatter_matrix.png"

# carregando dados e limpando coluna id (dencessaria)
df = pd.read_csv(dataset)
cs = ["f"+str(x) for x in range(len(df.columns)-2)] + ["sample", "subject"]
df.columns = cs
df = df.drop(["sample"], axis=1)

# matriz de correlacao
corr = df.drop(["subject"], axis=1).corr()
annot = corr.round(decimals=1)
seaborn.heatmap(corr, vmin=-1.0, vmax=1.0, linewidths=0.4, cmap="Purples", annot=annot)
plt.title("Correlação entre preditores")
plt.show()
plt.savefig(heatmap_file, bbox_inches="tight")

"""
# pairplot
g = seaborn.PairGrid(df, hue="subject")
g = g.map_diag(plt.hist, histtype="step", linewidth=4)
g = g.map_offdiag(plt.scatter, s=25)
handles = g._legend_data.values()
labels = g._legend_data.keys()
g.fig.legend(handles=handles, labels=labels, loc='upper center', ncol=len(classes))
g.fig.subplots_adjust(top=0.98, bottom=0.02)
g.savefig(pairplot_file, bbox_inches="tight")
"""
"""
# lista de preditores (exclui-se a coluna da classe)
predictors = list(df.drop(["subject"], axis=1))

# analise monovariada
# - histograma
# - media
# - desvio padrao
# - assimetria
columns = ["predictor", "mean", "std", "var", "skewness"]
monovariate = pd.DataFrame(columns=columns)
for p in predictors:
	fig = plt.figure()
	plt.hist(df[p], bins=5)
	plt.title("Análise monovariada: preditor {}".format(p))
	plt.grid(b=True)
	fig.savefig("{}hist_p-{}.png".format(figpath, p), bbox_inches="tight")
	plt.close(fig)

	mean = round(df[p].mean(), 4)
	std = round(df[p].std(), 4)
	var = round(df[p].var(), 4)
	skewness = round(df[p].skew(), 4)

	new_entry = pd.DataFrame([[p, mean, std, var, skewness]], columns=columns)
	monovariate = monovariate.append(new_entry)
	
	print("Análise global do preditor {} OK".format(p))

# salva resultados em arquivo
monovariate.to_csv(result_file, index=False)
"""
