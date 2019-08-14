import os
import time
import numpy as np
import pandas as pd
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
from sklearn.model_selection import GridSearchCV
from joblib import dump, load

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
"""
kbest = SelectKBest(f_classif, k=20)
kbest = kbest.fit(X_train, y_train)
selected_features = kbest.get_support()
X_train = kbest.fit_transform(X_train, y_train)
X_test = kbest.fit_transform(X_test, y_test)
X_val = kbest.fit_transform(X_val, y_val)
"""

# STANDARDIZATION
scaler = StandardScaler()
scaler.fit(X_train)
X_train = scaler.transform(X_train)
X_test = scaler.transform(X_test)
X_val = scaler.transform(X_val)

# SKEW REMOVAL
pt = PowerTransformer(method="yeo-johnson", standardize=False)
pt.fit(X_train)
X_train = pt.transform(X_train)
X_test = pt.transform(X_test)
X_val = pt.transform(X_val)

# TRAIN + VAL
X_trainval = np.concatenate([X_train, X_val])
y_trainval = pd.concat([y_train, y_val])
prefold = [-1 for x in range(X_train.shape[0])] + \
          [0 for x in range(X_val.shape[0])]

# HYPERPARAMETERS TUNING
parameters = {
	"hidden_layer_sizes" : [(100,), (200,), (300,), (400,), (500,), (600,)],
	"activation" : ["identity", "logistic", "tanh", "relu"],
	"alpha" : [0.001, 0.001, 0.01],
	"max_iter" : [1000]
}

ps = PredefinedSplit(prefold)
estimator = MLP()
clf = GridSearchCV(estimator, parameters, cv=ps, n_jobs=-1, verbose=1)
clf.fit(X_trainval, y_trainval)

dump(clf, "../results/mlp_gridsearch.joblib")
print(clf.best_params_)
print(clf.best_score_)

"""
# CLASSIFICATION
classifiers = {
	"KNN_manhattam" : KNN(p=1),
	"KNN_euclidean" : KNN(p=2),
	"SVM_radial"    : SVM(kernel="rbf", gamma="auto"),
	"SVM_linear"    : SVM(kernel="linear", gamma="auto"),
	"RandomForest"  : RandomForestClassifier(n_estimators=200),
	"MLP"           : MLP(hidden_layer_sizes=(1000, ), max_iter=1000)
}

ans = {
	"classifier" : [],
	"accuracy"   : [],
	"precision"  : [],
	"recall"     : [],
	"f1"         : [],
	"elapsed"    : []
}

print("CLASSIFIER\tACC\tPRE\tREC\tF1\tTIME")
for name, classifier in classifiers.items():
	start = time.time()
	
	classifier.fit(X_train, y_train)
	pred = classifier.predict(X_test)
	
	accuracy = round(accuracy_score(y_test, pred), 2)
	precision = round(precision_score(y_test, pred, average="micro"), 2)
	recall = round(recall_score(y_test, pred, average="micro"), 2)
	f1 = round(f1_score(y_test, pred, average="micro"), 2)
	
	elapsed = round(time.time() - start, 4)
	
	ans["classifier"].append(name)
	ans["accuracy"].append(accuracy)
	ans["precision"].append(precision)
	ans["recall"].append(recall)
	ans["f1"].append(f1)
	ans["elapsed"].append(elapsed)
	
	print("{}\t{}\t{}\t{}\t{}\t{}".format(name,
	                                      accuracy,
	                                      precision,
	                                      recall,
	                                      f1,
	                                      elapsed))

ans = pd.DataFrame(ans)
ans.to_csv("../results/shapenet_ans.csv", index=None)
"""
