#include <stdio.h>
#include <limits.h>

// --- Definições Abstratas ---
// Você precisará definir o que compõe um Estado e uma Ação no seu jogo específico.
typedef struct
{
  char tabuleiro[9]; // tabuleiro de 3x3
} Estado;

typedef struct
{
  int linha, coluna; // posição da jogada
} Acao;

// --- Funções auxiliares ---
int teste_terminal(Estado estado);
int utilidade(Estado estado);
Acao *acoes(Estado estado, int *num_acoes);
Estado resultado(Estado estado, Acao acao);

// --- Funções utilitárias matemáticas ---
int max(int a, int b) { return (a > b) ? a : b; }
int min(int a, int b) { return (a < b) ? a : b; }

// --- Algoritmo Minimax ---
int minimax(Estado estado, int profundidade, int e_maximizador)
{
  // Caso base: verifica se o nó é terminal (fim de jogo)
  if (teste_terminal(estado))
  {
    return utilidade(estado);
  }

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

    // Nota: Deve liberar a memória de lista_acoes
    // caso ela tenha sido alocada dinamicamente.
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

    return pior_valor;
  }
}