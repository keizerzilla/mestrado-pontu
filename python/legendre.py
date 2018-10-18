import sys
import seaborn
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from sklearn.preprocessing import PowerTransformer

if len(sys.argv) != 3:
	print("erro num param!")
	print("uso: legendre.py <arquivo_feat.dat> <arquivo_saida.dat>")
	sys.exit()

df = pd.read_csv(sys.argv[1])
cs = ["f"+str(x) for x in range(len(df.columns)-2)] + ["sample", "subject"]
df.columns = cs

# removendo correlatos
#del df["f26"] # esse eh bom! 84.97!

corr_matrix = df.drop(["sample", "subject"], axis=1).corr().abs()
up = corr_matrix.where(np.triu(np.ones(corr_matrix.shape), k=1).astype(np.bool))
to_drop = [column for column in up.columns if any(up[column] > 0.9)]
print(list(to_drop))
#ft = df.drop(df.columns[to_drop], axis=1)


del df["f5"]
del df["f6"]
del df["f7"]
del df["f8"]
del df["f10"]
del df["f11"]
del df["f12"]
del df["f14"]
del df["f15"]
del df["f18"]
del df["f19"]

"""
predictors = list(df.drop(["sample", "subject"], axis=1))
trans = pd.DataFrame(columns=list(df))
for p in predictors:
	pt = PowerTransformer(method="yeo-johnson")
	data = np.array(df[p]).reshape(-1, 1)
	pt.fit(data)
	ans = pd.DataFrame(data=pt.transform(data), index=data)
	trans.loc[:, p] = ans[0].values.tolist()

trans.loc[:, "sample"] = df["sample"]
trans.loc[:, "subject"] = df["subject"]
trans.to_csv(sys.argv[2], index=False, header=None)
"""

df.to_csv(sys.argv[2], index=False, header=None)

