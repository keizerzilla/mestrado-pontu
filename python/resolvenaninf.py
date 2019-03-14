from naninf import *
from features import *

# captura amostras de erro da planilha de resultados
data_analysis = pd.read_csv("../results/naninf.csv")
samples = []
for index, row in data_analysis.iterrows():
	m = row["moment"]
	s = "../datasets/"+row["scenario"]+"/"+row["face"]+"/"+row["sample"]+".xyz"
	c = row["cut"][0]
	target = {"moment" : m, "cloud" : s, "cut" : c}
	samples.append(target)

# recompila codigo do extrator de atributos
cmd = ["make", "mcalc", "-C", "../src/"]
print(subprocess.run(cmd, stdout=subprocess.PIPE).stdout.decode("utf-8"))

# executa extracoes e analise de erros
me = MomentExtractor()
for sample in samples:
	if sample["moment"] == "zernike":
		#print(sample["moment"], sample["cloud"], sample["cut"])
		ans = me.cloudExtraction(sample["moment"],sample["cloud"],sample["cut"])
		print(ans)
		
