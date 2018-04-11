#!/usr/bin/python3
#!python3

import csv
import sys
import time
import numpy as np
import pandas as pd
from sklearn.svm import SVC
from sklearn.neighbors import KNeighborsClassifier

# funcao de classificacao
def classif(ext_name, cutsize, training_ft, training_cl, test_ft, test_cl):
    classifiers = [
        KNeighborsClassifier(p=1, n_neighbors=1),
        KNeighborsClassifier(p=2, n_neighbors=1),
        SVC(kernel="rbf"),
        SVC(kernel="poly")]

    names = [
        "KNN_manhattam",
        "KNN_euclidean",
        "SVM_radial",
        "SVM_poly"]

    X_train = np.array(pd.read_csv(training_ft, header=None))
    y_train = np.ravel(pd.read_csv(training_cl, header=None))
    X_test = np.array(pd.read_csv(test_ft, header=None))
    y_test = np.ravel(pd.read_csv(test_cl, header=None))
    
    scores = []
    for clf_name, clf in zip(names, classifiers):
        start = time.time()
        clf.fit(X_train, y_train)
        score = clf.score(X_test, y_test)
        elapsed = round(time.time() - start, 4)
        score_txt = str(round(score*100, 2))
        result = "{:<16}{:<16}{:<16}{:<16}".format(ext_name, clf_name, score_txt, elapsed)
        scores.append(score_txt)
        print(result)
    with open("{}".format(ext_name), "a+") as dump:
        dump.write(str(max(scores)) + "\n")
    

# ensina a usar o script
def usage():
	print("Uso: <ext_name> <cutsize> <training_ft> <training_cl> <test_ft> <test_cl>\n")
	print("<ext_name>: nome do extrator usado para gerar os atributos")
	print("<cutsize>: tamanho do corte aplicado na nuvem")
	print("<training_ft>: arquivo com os atributos de treinamento")
	print("<training_cl>: arquivo com as classes dos atributos de treinamento")
	print("<test_ft>: arquivo com os atributos de teste")
	print("<test_cl>: arquivo com as classes dos atributos de teste")

# main function
def main():
	argc = len(sys.argv)
	if argc == 7:
		classif(sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4], sys.argv[5], sys.argv[6])
	else:
		usage()

# main code
main()
