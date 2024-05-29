# Projeto de Introdução a Programação Paralela e Distribuída

---

Integrantes:

- João Henrique Botelho, 201154048
- João Vitor Figueredo, 201154072
- João Pedro Brum, 201152428

## Instruções

Como projeto final seu grupo deve paralelizar uma aplicação usando algumas das linguagens vistas durante a disciplina. Também são aceitas outras linguagens, mas a responsabilidade pelo correto entendimento do suporte de paralelismo da linguagem escolhida é do grupo.

## Projeto Escolhido - Diagrama de voronoi

Diagrama de Voronoi é a decomposição espacial, neste projeto, de um plano cartesiano em setores de voronoi (ou celulas Dirichlet), realizando a definição das bordas do setor utilizando o bi-setor dos pontos presentes em um dado set. 

![Exemplo](Images\Exemplo_Diagrama_Voronoi.png "Exemplo")

## Algoritimo

O algoritmo segue a seguintes etapas. Primeiro, se define as bordas do Plano cartesiano que emglobe todos os pontos no set

![Passo 1](Images\Step_1.png "Passo 1")

**(Tome P1 como P e P2 como Q)**

E depois se inicia uma iteração dos n pontos no set em que dado o ponto P, a cada ponto Q se segue a seguinte rotina:

- Primeiro se define como borda do setor em P a borda do plano cartesiano.

![Passo 2](Images\step_n_1.png "Passo 2")

- Acha o Bi-setor de P e Q e defina os pontos de intersecção (pode haver de 0 a 2) com as bordas do setor em P.

![Passo 3](Images\step_n_2.png "Passo 3")

- Retira-se os pontos que estiverem fora da parte do setor que contem o ponto P.

![Passo 4](Images\step_n_3.png "Passo 4")

- Define-se o novo setor.

![Passo 5](Images\step_n_4.png "Passo 5")

- Repita esse processo com todos os demais pontos no set.

![Q em p3](Images\step_2.png "Q em p3")

![Q em p4](Images\step_3.png "Q em p4")
- Setor em P = (T6,A,T1,T3,T5)
- No final adicione o setor ao array de setores.

##### Pseudocódigo 

```
X_mn = valor x minimo do set
X_mx = valor x maximo do set
Y_mn = valor y minimo do set
Y_mx = valor y maximo do set

padding = valor para que nenhum ponto do set caia na linha do limite do plano cartesiano

Box = {(X_mn + padding,Y_mn + padding),(X_mx + padding,Y_mn + padding),(X_mx + padding,Y_mx + padding),(X_mn + padding,Y_mx + padding)}

Para cada ponto P em set faça:
    setor_p = Box 
    Para cada Ponto Q em set faça:
        Bi = Bi-setor(P,Q)
        Para cada borda b em setor_p faça:
            secção = intersecção(Bi,b)
            Se houver intersecção:
                setor_p.push(intersecção)
                setor_p.pop(pontos_fora)
                setot_p = sort_no_sentido_anti-horario(P,setor_p)
    setores.push(setor_p)
                
```

## Exemple

##### Input  
```
//Cada par de valores é um ponto
Point_set = {50, 100, 106, 49, 66, 175, 137, 197, 195, 147, 178, 73, 123, 123}
```

##### Output  
```v
Celula: 0
[0,149.873] [79.7716,132.855] [92.9764,90.9446] [10.1518,-0] [0,0]
Celula: 1
[92.9764,90.9446] [135.256,81.2317] [162.333,-0] [10.1518,-0]
Celula: 2
[0,200] [97.162,200] [108.283,164.109] [79.7716,132.855] [-0,149.873]
Celula: 3
[97.162,200] [190.138,200] [152.058,155.827] [108.283,164.109]
Celula: 4
[152.058,155.827] [190.138,200] [200,200] [200,106.899] [165.744,114.768] 
Celula: 5
[135.256,81.2317] [165.744,114.768] [200,106.899] [200,0] [162.333,0]
Celula: 6
[79.7716,132.855] [108.283,164.109] [152.058,155.827] [165.744,114.768] [135.256,81.2317] [92.9764,90.9446]
```

##### Desmonstração Grafica
![Passo 2](Images\Resultadografico.png "Passo 2")

## FONTES
https://youtu.be/I6Fen2Ac-1U?si=YycZ3wpFmRtNuTD3
https://math.stackexchange.com/questions/361412/finding-the-angle-between-three-points