"""
boxplot.py
Artur Rodrigues Rocha Neto
2018

Script simples que mostra gera boxplot das variações de taxa de acerto de uma
dada combinação de momentos.
"""

import os
import sys
import numpy as np
import pandas as pd
import seaborn as sn
import matplotlib.pyplot as plt

if len(sys.argv) != 2:
	print("Número incorreto de parâmetros! Uso: boxplot.py <pasta_combos.csv>")
	sys.exit()

data_path = sys.argv[1]

replace_dict = {"bosphorus" : "bs",
                "outlier" : "out",
                "densit" : "d",
                "crop" : "c"}

scenarios = ["bosphorus.csv",
			 "bosphorus-outlier.csv",
			 "bosphorus-outlier-densit200.csv",
			 "bosphorus-outlier-densit200-crop60.csv",
			 "bosphorus-outlier-densit200-crop70.csv",
			 "bosphorus-outlier-densit200-crop80.csv",
			 "bosphorus-outlier-densit200-crop60-icp.csv",
	         "bosphorus-outlier-densit200-crop70-icp.csv",
	         "bosphorus-outlier-densit200-crop80-icp.csv",
			 "bosphorus-outlier-densit225.csv",
			 "bosphorus-outlier-densit225-crop60.csv",
			 "bosphorus-outlier-densit225-crop70.csv",
			 "bosphorus-outlier-densit225-crop80.csv",
	         "bosphorus-outlier-densit225-crop60-icp.csv",
	         "bosphorus-outlier-densit225-crop70-icp.csv",
	         "bosphorus-outlier-densit225-crop80-icp.csv"]

scenarios = [data_path + s for s in scenarios]

xaxes = []
for data in scenarios:
	for old, new in replace_dict.items():
		data = os.path.basename(data.replace(old, new))
	xaxes.append(data)
xaxes = [""] + xaxes

sn.set()
tick_marks_x = np.arange(len(xaxes))
boxes = []
for data in scenarios:
	print(data)
	df = pd.read_csv(data)
	boxes.append(df["rate"])

fig, ax = plt.subplots()
ax.boxplot(boxes)
plt.xticks(tick_marks_x, xaxes, rotation=90)
plt.title("Variação das taxas de acerto rank1_neutro para diferentes configs")
plt.tight_layout()
plt.show()

