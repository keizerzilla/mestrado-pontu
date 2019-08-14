import os
import time
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from sklearn.svm import SVC as SVM
from sklearn.metrics import f1_score
from sklearn.metrics import recall_score
from sklearn.naive_bayes import GaussianNB
from sklearn.metrics import accuracy_score
from sklearn.metrics import precision_score
from sklearn.metrics import confusion_matrix
from sklearn.ensemble import AdaBoostClassifier
from sklearn.tree import DecisionTreeClassifier
from sklearn.preprocessing import StandardScaler
from sklearn.preprocessing import PowerTransformer
from sklearn.ensemble import RandomForestClassifier
from sklearn.neighbors import NearestCentroid as NC
from sklearn.neural_network import MLPClassifier as MLP
from sklearn.neighbors import KNeighborsClassifier as KNN
from sklearn.discriminant_analysis import LinearDiscriminantAnalysis as LDA
from sklearn.discriminant_analysis import QuadraticDiscriminantAnalysis as QDA

from sklearn.model_selection import PredefinedSplit
from sklearn.feature_selection import f_classif
from sklearn.model_selection import GridSearchCV
from sklearn.feature_selection import SelectKBest
from sklearn.decomposition import PCA
from joblib import dump
from joblib import load

# FILES
datFile = "../results/shapenet_zernike_odd.dat"
splitFile = "/home/keizerzilla/SNCV2/train_test_split.csv"

# LOADING DATA FILE
df = pd.read_csv(datFile, header=None)
cols = ["z{}".format(x) for x in range(len(df.columns)-2)] + ["sample", "class"]
df.columns = cols

# LOADING TRAIN_TEST_VALIDATION SPLIT FILE
split = pd.read_csv(splitFile)
split = split.drop(["id", "synsetId", "subSynsetId"], axis=1)

# SETTING SPLIT VARIABLES 1
train = split.loc[split["split"] == "train"]
test = split.loc[split["split"] == "test"]
val = split.loc[split["split"] == "val"]

# SETTING SPLIT VARIABLES 2
train_set = df.loc[df["sample"].isin(train["modelId"])]
test_set = df.loc[df["sample"].isin(test["modelId"])]
val_set = df.loc[df["sample"].isin(val["modelId"])]

# SETTING SPLIT VARIABLES 3
X_train = train_set.drop(["sample", "class"], axis=1)
y_train = train_set["class"]
X_test = test_set.drop(["sample", "class"], axis=1)
y_test = test_set["class"]
X_val = val_set.drop(["sample", "class"], axis=1)
y_val = val_set["class"]

# FEATURE SELECTION
scores = []
start = 2
stop = 111
for k in range(start, stop):
	kbest = SelectKBest(f_classif, k=k)
	kbest = kbest.fit(X_train, y_train)
	selected_features = kbest.get_support()
	X_train_k = kbest.fit_transform(X_train, y_train)
	X_test_k = kbest.fit_transform(X_test, y_test)
	X_val_k = kbest.fit_transform(X_val, y_val)
	
	# STANDARDIZATION
	scaler = StandardScaler()
	scaler.fit(X_train_k)
	X_train_k = scaler.transform(X_train_k)
	X_test_k = scaler.transform(X_test_k)
	X_val_k = scaler.transform(X_val_k)

	# SKEW REMOVAL
	pt = PowerTransformer(method="yeo-johnson", standardize=False)
	pt.fit(X_train_k)
	X_train_k = pt.transform(X_train_k)
	X_test_k = pt.transform(X_test_k)
	X_val_k = pt.transform(X_val_k)
	
	clf = RandomForestClassifier(n_estimators=300)
	clf.fit(X_train_k, y_train)
	score = round(clf.score(X_test_k, y_test)*100, 2)
	scores.append(score)
	
	print(k, score)

plt.plot(range(start, stop), scores)
plt.title("Accuracy by number of selected features by ANOVA")
plt.tight_layout()
plt.show()

