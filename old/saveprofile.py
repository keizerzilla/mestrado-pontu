import os
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

result_dir = "result/"
bigdata = np.array([])

for r in os.listdir(result_dir):
    df = np.array(pd.read_csv(result_dir + r, header=None))
    bigdata = np.append(bigdata, df)
    print(r + " done!")

print("Salvando dados em arquivo...")
np.savetxt("neutral.txt", bigdata, fmt="%.64f")
print("Dados salvos!")