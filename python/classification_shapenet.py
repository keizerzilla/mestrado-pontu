import pandas as pd
from sklearn.decomposition import PCA
from sklearn.preprocessing import StandardScaler
from sklearn.preprocessing import PowerTransformer
from sklearn.neighbors import NearestCentroid as NC
from sklearn.neighbors import KNeighborsClassifier as KNN
from sklearn.discriminant_analysis import LinearDiscriminantAnalysis as LDA

# FILES
datFiles = ["../results/shapenet_zernike_odd.dat",
            "../results/shapenet_zernike_even.dat",
            "../results/shapenet_zernike_mag.dat"]

splitFile = "../shapenet/all.csv"

# LOADING TRAIN_TEST_VALIDATION SPLIT FILE
split = pd.read_csv(splitFile)
split = split.drop(["id", "synsetId", "subSynsetId"], axis=1)

# SETTING SPLIT VARIABLES
train = split.loc[split["split"] == "train"]
test = split.loc[split["split"] == "test"]
val = split.loc[split["split"] == "val"]

for datFile in datFiles:
    # LOADING DATA FILE
    df = pd.read_csv(datFile, header=None)
    n_features = len(df.columns) - 2
    n_samples = df.shape[0]
    pca_lim = min(n_samples, n_features)
    
    feats = ["z{}".format(x) for x in range(n_features)]
    cols = feats + ["sample", "class"]
    df.columns = cols
    
    # SPLITIN SETS
    train_set = df.loc[df["sample"].isin(train["modelId"])]
    test_set = df.loc[df["sample"].isin(test["modelId"])]
    val_set = df.loc[df["sample"].isin(val["modelId"])]
    
    ans = {
    	"n"        : [],
    	"accuracy" : [],
    }
    
    X_train = train_set.drop(["sample", "class"], axis=1)
    y_train = train_set["class"]
    X_test = test_set.drop(["sample", "class"], axis=1)
    y_test = test_set["class"]
    X_val = val_set.drop(["sample", "class"], axis=1)
    y_val = val_set["class"]
    
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
    
    for n in range(1, pca_lim+1):
        # PCA
        pca = PCA(n_components=n, svd_solver="full")
        pca.fit(X_train)
        X_train_pca = pca.transform(X_train)
        X_test_pca = pca.transform(X_test)
        X_val_pca = pca.transform(X_val)
        
        # CLASSIFICATION
        classifier = NC()
        classifier.fit(X_train_pca, y_train)
        accuracy = classifier.score(X_val_pca, y_val)
        
        accuracy = round(100 * accuracy, 2)
        
        ans["n"].append(n)
        ans["accuracy"].append(accuracy)
        
        print("{}\t{}\t{}".format(datFile, n, accuracy))
        
        #ans = pd.DataFrame(ans)
        #ans.to_csv("../results/shapenet_ans.csv", index=None)
    
    # SAVE HERE
    