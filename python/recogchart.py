import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

df = pd.read_csv("../results/doc/classification.csv")
x = list(df["dataset"])

df.plot.line()
plt.xticks(range(len(x)), x, rotation=90)
plt.grid()
plt.show()

