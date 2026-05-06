# Análise Comparativa: Minimax Puro vs. Minimax com Poda Alfa-Beta

## 1. Tabela de Nós Visitados

Os valores abaixo foram coletados durante uma partida real com o computador jogando primeiro (tabuleiro vazio na 1ª jogada). Os contadores foram zerados antes de cada chamada ao algoritmo.

| Jogada do Computador | Células Vazias | Minimax Puro | Poda Alfa-Beta | Redução |
|:--------------------:|:--------------:|:------------:|:--------------:|:-------:|
| 1ª (tabuleiro vazio) | 9              | 549.945      | 18.296         | 96,7%   |
| 2ª                   | 7              | 8.231        | 748            | 90,9%   |
| 3ª                   | 5              | 173          | 77             | 55,5%   |

A partida que gerou esses valores seguiu a sequência abaixo (computador = X, jogador = O):

| Rodada | Jogada do Computador | Jogada do Jogador | Estado do Tabuleiro       |
|:------:|:--------------------:|:-----------------:|:-------------------------:|
| 1      | (1, 1)               | (1, 2)            | `X O .` / `. . .` / `. . .` |
| 2      | (2, 1)               | (3, 3)            | `X O .` / `X . .` / `. . O` |
| 3      | (2, 2)               | —                 | `X O .` / `X X .` / `. . O` |

> **Nota:** o jogo também exibe esta tabela ao final de cada partida. Os valores variam conforme as jogadas do usuário, pois o número de células vazias e o perfil do tabuleiro mudam.

---

## 2. Diferença Observada vs. Complexidade Teórica

### Complexidade dos algoritmos

| Algoritmo               | Complexidade (pior caso)   | Para Jogo da Velha (b≈9, m=9)        |
|-------------------------|----------------------------|---------------------------------------|
| Minimax Puro            | O(b^m)                     | ~9^9 ≈ 387 milhões (nós teóricos)    |
| Minimax com Alfa-Beta   | O(b^(m/2)) — caso médio    | ~9^4,5 ≈ 19.683 (nós teóricos)       |

Onde **b** é o fator de ramificação (número de ações possíveis) e **m** é a profundidade máxima da árvore.

### Resultados observados

Na 1ª jogada, o Minimax Puro visitou **549.945 nós**. Esse número é bem menor que o valor teórico de 9^9 ≈ 387 milhões porque vários ramos terminam antes da profundidade máxima (vitórias antecipadas), reduzindo a árvore real.

O Alfa-Beta visitou **18.296 nós** na mesma jogada — valor próximo ao que a teoria prevê para o caso médio, O(b^(m/2)) ≈ 9^4,5 ≈ 19.683. Isso mostra que, na prática, o Alfa-Beta consegue descartar uma parte expressiva da árvore sem perder a decisão ótima, chegando a um resultado equivalente ao Minimax Puro com muito menos nós visitados.

À medida que o tabuleiro se preenche (jogadas 2 e 3), a diferença absoluta cai porque a árvore de busca já é menor para ambos os algoritmos. A redução percentual também diminui, o que é esperado: com poucas células vazias, há menos ramos para podar.

---

## 3. Efeito da Ordenação das Ações sobre as Podas

### Ordenação atual

A função `acoes()` percorre as posições de 0 a 8 (linha a linha, da esquerda para a direita). Isso significa que o centro (posição 4) só é considerado na quinta ação de uma lista completa, e as arestas laterais ficam intercaladas com os cantos.

### Por que a ordem importa

A poda Alfa-Beta só descarta um ramo quando o valor já garantido pelo jogador atual supera o que o adversário permitiria. Quanto **melhores forem as ações analisadas primeiro**, mais cedo os limites α e β se estreitam, e mais ramos subsequentes são podados.

No Jogo da Velha, as posições de maior valor estratégico são:

1. **Centro** (posição 4) — controla quatro linhas simultâneas.
2. **Cantos** (posições 0, 2, 6, 8) — participam de três linhas cada.
3. **Arestas** (posições 1, 3, 5, 7) — participam de apenas duas linhas.

### Impacto esperado

Se a função `acoes()` fosse reescrita para priorizar centro → cantos → arestas, o algoritmo encontraria jogadas de alto valor logo no início da iteração. Com α e β mais apertados desde o começo, um número maior de subárvores seria descartado antes de ser explorado, reduzindo a contagem de nós visitados em relação aos 18.296 medidos na 1ª jogada.

Com uma ordenação estratégica, o algoritmo encontraria jogadas de alto valor logo no início, estreitando os limites α e β mais cedo e descartando mais ramos — o que reduziria a contagem de nós em relação aos 18.296 medidos na 1ª jogada.

Com a ordenação atual, os nós visitados na **pior ordenação possível** (arestas primeiro, cantos depois, centro por último) seriam consideravelmente maiores, podendo se aproximar dos valores do Minimax Puro, pois as podas raramente seriam ativadas cedo o suficiente para eliminar ramos significativos.
