import numpy as np
import pandas as pd

df = pd.read_csv("aly_dist.csv")
print(len(df["subject"].unique()))
print(list(df.columns))

for sub in df["subject"].unique():
	subject = df.loc[df["subject"] == sub]
	for crop in [20, 40, 60]:
		points = subject.loc[subject["crop"] == crop][["x","y","z"]]
		points = np.array(points)
		print("{} - {}".format(sub, crop))
		print(np.cov(points))


