import os
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from sklearn.linear_model import LinearRegression

src = "/home/artur/LATIN/centerprofile/nonneutral_sort/"
dst = "/home/artur/LATIN/centerprofile/40_60/"
regr = LinearRegression()

def reglinear(data):
	sample_size = -1 * int(data.size/2)
	x_train = data[:sample_size]
	x_test = data[sample_size:]
	
	base = np.arange(data.size)
	y_train = base[:sample_size]
	y_test = base[sample_size:]
	
	regr.fit(x_train, y_train)
	return regr.predict(x_test), regr.coef_

coefs = []
for cloud in os.listdir(src):
	path = src + cloud
	df = np.array(pd.read_csv(path, header=None))
	
	df = np.array([y for y in df if y >= 40 and y <= 60])
	np.savetxt(dst + cloud, df)
	df, coef = reglinear(df)
	coefs.append(coef)
	
	plt.plot(df)
	print(path + " OK")

coefs = np.array(coefs)
np.savetxt(dst + "coefs.txt", coefs)
plt.show()
