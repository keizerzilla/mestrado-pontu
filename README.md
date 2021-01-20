# pontu: Uma biblioteca para Nuvens de Pontos
## Versão histórica

Este repositório começou como o acervo de todos os códigos, ensaios e resultados que eu geraria ao longo do meu Mestrado em Engenharia de Teleinformática. Entretanto, um dos produtos concebidos nesse período foi um conjunto de códigos em C para análise e prototipagem rápida de experimentos com Nuvens de Pontos. A motivação por trás de escrever um conjunto de funções básicas já encontradas em outras bibliotecas, principalmente na PCL (Point Cloud Library), surgiu da dificuldade crescente a cada versão dessas bibliotecas em instala-las e compilar o mais simples dos programas; a quantidade de dependências e de funcionalidades desnecessárias aos meus problemas de pesquisa específicos tornavam contraprodutivo seu uso. Assim, resolvi escrevê-las eu mesmo. Aos poucos essas pequena biblioteca autoral foi criando volume, mas sua evolução aconteceu respeitando sempre três regras importantes de desenho:

- Sem dependências a não ser a biblioteca padrão do C na sua versão 11 (C-ANSI-11)
- Sem divisão em subconjuntos (funcionalidades disso, funcionalidades daquilo)
- Precisava ser rápida (motivação a criar código otimizado) e de fácil compilação (motivação a criar código de fácil manutenção)

Hoje a biblioteca ganhou tamanho e importância além da minha pesquisa em particular, finalizada em dezembro de 2020. O repositório atualizado dela continua privado em um outro ambiente até que sua publicação oficial seja feita, em algum ponto de 2021. Esta versão no GitHub não contém as mais novas funcionalidades nem as últimas otimizações efetuadas em diversas estruturas do código, portanto eu a considero uma versão a parte apenas de caráter histórico. Estou tornando-a pública hoje, 10 de janeiro de 2021, sabendo que ela pode servir a outros como fonte de inspiração. A versão oficial será de código-livre e convidará a comunidade de Visão Computacional a colaborar com a sua evolução. Quando do anúncio dela, eu atualizarei esse readme com o endereço para acesso.

- Este trabalho foi parcialmente financiado pela CAPES: Coordenação de Aperfeiçoamento de Pessoal de Nível Superior.
