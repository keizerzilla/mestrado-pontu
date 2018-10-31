"""
recogchart.py
Artur Rodrigues Rocha Neto
2018

Plota resultados de reconhecimento para todos os momentos e todos os datasets.
"""

import os
import sys
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

if len(sys.argv) != 2:
	print("Número incorreto de parâmetros! Uso: recogchart.py result.csv")
	sys.exit()

df = pd.read_csv(sys.argv[1])
config = os.path.basename(sys.argv[1]).replace(".csv", "")
ax = df.plot(marker='o')
tick_marks_x = np.arange(len(df["dataset"]))
tick_marks_y = range(0, 110, 10)
plt.xticks(tick_marks_x, df["dataset"], rotation=45)
plt.yticks(tick_marks_y)
plt.title("Taxa de acerto na configuração " + config)
plt.ylabel("Acerto (%)")
plt.grid()

for moment in list(df.drop(["dataset"], axis=1)):
	for x, y in enumerate(df[moment]):
		ax.annotate(str(y), xy=(x, y), xytext=(x-0.2, y+1))

plt.tight_layout()
print(df)
plt.show()

