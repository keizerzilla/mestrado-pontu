# Nessa pasta tem e3 scripts pra usar.  
 - 1 registro_neutrals.sh
 - 2. registro_database.sh
 - 3. registro.sh

# Tem uma pasta que ta em  ./data_test/DATA ,  nela tem os dados que a jamile gerou e q eu padronizei pra colocar no servidor. 

# Basicamente, o registro_neutrals.sh   entra dentro de uma pasta que contem a pasta com a base (BOSPHORUS_PCD) e a pasta com os landmarks (LANDMARKS_PCD_CORRECTION) e gera uma outra pasta chamada  NEUTRALS_REGISTRATION com os Neutrals alinhados.  

# O registro_database.sh tb faz isso  e alinha as nuvens de cada indivíduo com base nos Neutrals gerados, gerando uma nova pasta chamada  REGISTRATION. 

# O script registro.sh chama os dois passando uma determinada pasta que contenha BOSPHORUS_PCD e LANDMARKS_PCD_CORRECTION . 

# Então tudo que tem a fazer  assim que baixar a pasta aqui  é rodar o script registro.sh.  Dentro dele vc vai ver que tem um exemplo lá que faz pra primeira pasta que é a Passo3.A.1_OutlierRemovedDownsample2.00Crop60,  aí tu pode rodar e ver que ele gerou as duas pastas (NEUTRALS_REGISTRATION e REGISTRATION) dentro dessa aí. 

# Aí já tem um for lá que ta pegando cada pasta dentro de DATA e fazendo isso.

# Dentro das pastas  NEUTRALS_REGISTRATION e REGISTRATION ele gera uma pasta indicando o offset que eu fiz,  e dentro dela gera 3 pastas, uma com a base alinhada, outra com os landmarks alinhados, e outra so com o nariz alinhado.

