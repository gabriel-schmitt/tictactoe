#include <stdio.h>
#include <ctype.h>
#include <limits.h>
#include <stdlib.h>

// --- Definições Abstratas ---
typedef struct
{
  char tabuleiro[9]; // tabuleiro de 3x3
} Estado;

typedef struct
{
  int linha, coluna; // posição da jogada
} Acao;

struct
{
  char max, min;
} xisOuBola;



// --- Funções auxiliares ---
int teste_terminal(Estado estado)
{
  if (utilidade(estado) != 0)
    return 1; // Jogo terminou com um vencedor

  for (int i = 0; i < 9; i++)
    if (estado.tabuleiro[i] == ' ')
      return 0;

  return 1;
}

int utilidade(Estado estado)
{
  // Retorna +1 se o jogador MAX ganhou, -1 se o jogador MIN ganhou, ou 0 para empate
  // Você precisará implementar a lógica para determinar o vencedor com base no estado do tabuleiro.
  for (int i = 0; i < 3; i++)
  {
    // Verificar linhas
    if (estado.tabuleiro[i * 3] != ' ' && estado.tabuleiro[i * 3] == estado.tabuleiro[i * 3 + 1] && estado.tabuleiro[i * 3 + 1] == estado.tabuleiro[i * 3 + 2])
    {
      return (estado.tabuleiro[i * 3] == xisOuBola.max) ? 1 : -1;
    }
    // Verificar colunas
    if (estado.tabuleiro[i] != ' ' && estado.tabuleiro[i] == estado.tabuleiro[i + 3] && estado.tabuleiro[i + 3] == estado.tabuleiro[i + 6])
    {
      return (estado.tabuleiro[i] == xisOuBola.max) ? 1 : -1;
    }
  }
  // Verificar diagonais
  if (estado.tabuleiro[0] != ' ' && estado.tabuleiro[0] == estado.tabuleiro[4] && estado.tabuleiro[4] == estado.tabuleiro[8])
  {
    return (estado.tabuleiro[0] == xisOuBola.max) ? 1 : -1;
  }
  if (estado.tabuleiro[2] != ' ' && estado.tabuleiro[2] == estado.tabuleiro[4] && estado.tabuleiro[4] == estado.tabuleiro[6])
  {
    return (estado.tabuleiro[2] == xisOuBola.max) ? 1 : -1;
  }

  return 0; // Empate ou jogo em andamento
}

Acao *acoes(Estado estado, int *num_acoes)
{
  // Retorna uma lista de ações possíveis (jogadas) para o estado atual
  // Implementar a lógica para gerar as ações com base no estado do tabuleiro.
  Acao *lista_acoes = (Acao *)malloc(sizeof(Acao) * 9);
  if (lista_acoes == NULL) exit(1);

  *num_acoes = 0;
  for (int i = 0; i < 9; i++)  {
    if (estado.tabuleiro[i] == ' ')
    {
      lista_acoes[*num_acoes].linha = i / 3;
      lista_acoes[*num_acoes].coluna = i % 3;
      (*num_acoes)++;
    }
  }
  return lista_acoes;
}

Estado resultado(Estado estado, Acao acao);

// --- Funções utilitárias matemáticas ---
int max(int a, int b) { return (a > b) ? a : b; }
int min(int a, int b) { return (a < b) ? a : b; }

// --- Algoritmo Minimax ---
int minimax(Estado estado, int profundidade, int e_maximizador)
{
  // Caso base: verifica se o nó é terminal (fim de jogo)
  if (teste_terminal(estado))
    return utilidade(estado);

  // Obtém as ações possíveis para o estado atual
  int num_acoes = 0;
  Acao *lista_acoes = acoes(estado, &num_acoes);

  if (e_maximizador)
  {
    int melhor_valor = INT_MIN;

    for (int i = 0; i < num_acoes; i++)
    {
      Estado proximo_estado = resultado(estado, lista_acoes[i]);
      int valor = minimax(proximo_estado, profundidade + 1, 0);
      melhor_valor = max(melhor_valor, valor);
    }

    free(lista_acoes);
    return melhor_valor;
  }
  else
  {
    int pior_valor = INT_MAX;

    for (int i = 0; i < num_acoes; i++)
    {
      Estado proximo_estado = resultado(estado, lista_acoes[i]);
      int valor = minimax(proximo_estado, profundidade + 1, 1);
      pior_valor = min(pior_valor, valor);
    }

    free(lista_acoes);
    return pior_valor;
  }
}

int main() {
  printf("JOGO DA VELHA\n\n");

  do {
    printf("Quer ser X ou O?\n");
    xisOuBola.min = tolower(getc(stdin));
    xisOuBola.max = (xisOuBola.min == 'x') ? 'o' : 'x';
  } while (xisOuBola.min != 'x' && xisOuBola.min != 'o');

  return 0;
}