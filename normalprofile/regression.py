import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from sklearn.svm import SVR

print("Lendo dados...")
bigdata = np.array(pd.read_csv("neutral.txt", header=None))
bigdata = np.reshape(bigdata, -1)
bigdata = np.sort(bigdata)

print("Regressao...")
X = np.array(range(0, bigdata.size))
y = bigdata

print("Volume de dados: {}".format(X.shape))

#reshape
X = np.reshape(X, (-1, 1))
y = np.ravel(y)

#print("RADIAL")
#svr_rbf = SVR(kernel="rbf")
#y_rbf = svr_rbf.fit(X, y).predict(X)
print("LINEAR")
svr_lin = SVR(kernel="linear")
y_lin = svr_lin.fit(X, y).predict(X)
print("POLINOMIAL")
svr_poly = SVR(kernel="poly")
y_poly = svr_poly.fit(X, y).predict(X)

print("Plotando...")
lw = 2
plt.scatter(X, y, color="darkorange", label="data")
#plt.plot(X, y_rbf, color="navy", lw=lw, label="RBF model")
plt.plot(X, y_lin, color="c", lw=lw, label="Linear model")
plt.plot(X, y_poly, color="cornflowerblue", lw=lw, label="Polynomial model")
plt.xlabel("data")
plt.ylabel("target")
plt.title("Support Vector Regression")
plt.legend()
plt.show()

print("rbf coef: {}".format(svr_rbf.coef_))
print("lin coef: {}".format(svr_lin.coef_))
print("poly coef: {}".format(svr_poly.coef_))