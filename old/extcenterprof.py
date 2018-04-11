import os
import subprocess

cloud_dir = "data/bs_outliers_free/Neutrals/"

for c in os.listdir(cloud_dir):
    cloud = cloud_dir + c
    filename = c.split(".")[0]
    subprocess.run(["centerprofile", cloud, "result/" + filename + ".txt"])
    print(filename + " done!")
    
print("DONE")
