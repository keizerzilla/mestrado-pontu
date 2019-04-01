# Template para usar algumas funções em colaboração


# Para visualizar os exemplos das funções suportadas:
 - entre no arquivo CMakeLists e descomente a linha de codigo '#set(MAIN exemples.cpp)' e commente a linha de codigo 'set(MAIN template.cpp)'
 - Para utilizar o template em branco pra fabricar os próprios códigos faça o inverso do indicado acima.     

# Se usando o QT
 - mude o build do projeto para a pasta "build"

# Se usando o exemple_izaias.cpp
 - mude a pasta workspace para "data"
 - passe como argumento esse exemplo: "-clouds bs000_N_N_1.pcd,bs000_N_N_0.pcd -lm3 bs000_N_N_1.lm3,bs000_N_N_0.lm3 -outputs bs000_N_N_1_regist.pcd,bs000_N_N_1_nose_regist.pcd  -r 30 -icp 10,1,0.0005,200"		

# para criar suas  bibliotecas
 - Crie seus headers (.hpp)  e sources (.cpp) e coloque nas pastas include e src respectivamente
 - Após isso adicione o caminho dos arquivos no arquvio "CMakeLists" nas linhas de definição "set"	  
 - Depois é só incluí-lo em '#include "your_lib.hpp"' no arquvio que contem a função 'main'.  
