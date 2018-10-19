import os
import sys
import seaborn
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

"""
heatmap.py
Artur Rodrigues Rocha Neto
2018

Script simples que mostra matriz de correlação dos momentos invariantes em
formato "mapa de calor" (heatmap).
"""

if len(sys.argv) != 2:
	print("Número incorreto de parâmetros! Uso: heatmap.py <arquivo_feat.dat>")
	sys.exit()

# setando estilo e outras configs
seaborn.set()

# paths e arquivos
dataset = sys.argv[1]

# carregando dados e limpando coluna sample
df = pd.read_csv(dataset)
cs = ["f"+str(x) for x in range(len(df.columns)-2)] + ["sample", "subject"]
df.columns = cs
df = df.drop(["sample", "subject"], axis=1)

# matriz de correlacao e preditores de maior correlacao
corr = df.corr().abs()
annot = corr.round(decimals=2)
upper = corr.where(np.triu(np.ones(corr.shape), k=1).astype(np.bool))
to_drop = [column for column in upper.columns if any(upper[column] >= 0.9)]
super_corr = [column for column in upper.columns if any(upper[column] >= 0.98)]
print("drops sugeridos: " + str(to_drop))
print("super correlacoes: " + str(super_corr))

# plot
seaborn.heatmap(corr, vmin=0, vmax=1.0, cmap="Purples", annot=annot)
moment = os.path.basename(sys.argv[1]).split(".")[0]
plt.title("Correlação dos momentos " + moment)
plt.show()

