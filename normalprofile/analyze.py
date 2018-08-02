#!/usr/bin/python3
#!python3

import os
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

result_dir = "neutrals/"
bigdata = np.array([])

for r in os.listdir(result_dir):
    df = np.array(pd.read_csv(result_dir + r, header=None))
    df = np.reshape(df, -1)
    df = np.sort(df)
    plt.semilogy(df)
    
    bigdata = np.append(bigdata, df)
    print(r + " done!")

plt.title("Visualizacao logaritmica das distancias centrais - conj neutro")
plt.ylabel("distancia (mm)")
plt.xlabel("range de pontos")
plt.show()

'''
print("Lendo dados...")
bigdata = np.array(pd.read_csv("neutral.txt", header=None))
bigdata = np.reshape(bigdata, -1)
bigdata = np.sort(bigdata)

print("Regressao...")

print("Plotando...")
plt.scatter(range(0,bigdata.size), bigdata)
plt.title("Dispersao das distancias centrais - conj neutro")
plt.ylabel("distancia (mm)")
plt.xlabel("range de pontos")
plt.show()
'''
