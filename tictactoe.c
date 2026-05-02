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

// --- Protótipos das funções ---
int utilidade(Estado estado);
int teste_terminal(Estado estado);
Acao *acoes(Estado estado, int *num_acoes);
Estado resultado(Estado estado, Acao acao, int e_maximizador);
int max_value(Estado estado);
int min_value(Estado estado);
Acao minimax_decision(Estado estado);

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
  if (lista_acoes == NULL)
    exit(1);

  *num_acoes = 0;
  for (int i = 0; i < 9; i++)
  {
    if (estado.tabuleiro[i] == ' ')
    {
      lista_acoes[*num_acoes].linha = i / 3;
      lista_acoes[*num_acoes].coluna = i % 3;
      (*num_acoes)++;
    }
  }
  return lista_acoes;
}

Estado resultado(Estado estado, Acao acao, int e_maximizador)
{
  // Retorna o estado resultante de aplicar a ação no estado atual
  // Implementar a lógica para atualizar o tabuleiro com a jogada.
  Estado novo_estado = estado;
  int pos = acao.linha * 3 + acao.coluna;
  if (novo_estado.tabuleiro[pos] == ' ')
  {
    novo_estado.tabuleiro[pos] = (e_maximizador == 1) ? xisOuBola.max : xisOuBola.min;
  }
  return novo_estado;
}

// --- Funções utilitárias matemáticas ---
int max(int a, int b) { return (a > b) ? a : b; }
int min(int a, int b) { return (a < b) ? a : b; }

// --- Algoritmo Minimax ---
Acao minimax_decision(Estado estado)
{
  Acao best_action = {-1, -1};
  int best_value = INT_MIN; // Começamos com o pior valor possível para o MAX

  // 1. Iteramos por cada ação legal disponível no estado atual
  int num_acoes = 0;
  Acao *lista_acoes = acoes(estado, &num_acoes); // num_acoes é atualizado pela função `acoes`
  for (int i = 0; i < num_acoes; i++)
  {
    // 2. Simulamos o resultado da ação e perguntamos:
    // "Se eu fizer isso, qual o melhor que o oponente (MIN) pode fazer?"
    int v = min_value(resultado(estado, lista_acoes[i], 1));

    // 3. Se o valor retornado por essa linha de jogo (v) for melhor
    // do que o que encontramos até agora, atualizamos nossa escolha.
    if (v > best_value)
    {
      best_value = v;
      best_action = lista_acoes[i];
    }
  }

  // 4. Após testar todas as ações, retornamos a que resultou no maior v
  return best_action;
}

int max_value(Estado estado)
{
  // Tenta maximizar o ganho do agente
  // Caso base: se o jogo acabou, retorna a utilidade real
  if (teste_terminal(estado))
    return utilidade(estado);

  int v = INT_MIN; // Começamos com o pior valor possível para o MAX
  // Explora os sucessores: como é a vez do MAX, ele busca o maior valor entre os filhos
  int num_acoes = 0;
  Acao *lista_acoes = acoes(estado, &num_acoes);
  for (int i = 0; i < num_acoes; i++)
  {
    v = max(v, min_value(resultado(estado, lista_acoes[i], 0)));
  }
  return v;
}

int min_value(Estado estado)
{
  // Tenta minimizar o ganho do agente
  // Caso base: se o jogo acabou, retorna a utilidade real
  if (teste_terminal(estado))
    return utilidade(estado);

  int v = INT_MAX; // Começamos com o pior valor possível para o MIN
  // Explora os sucessores: como é a vez do MIN, ele busca o menor valor entre os filhos
  int num_acoes = 0;
  Acao *lista_acoes = acoes(estado, &num_acoes);
  for (int i = 0; i < num_acoes; i++)
  {
    v = min(v, max_value(resultado(estado, lista_acoes[i], 1)));
  }
  return v;
}

// --- UI ---
void exibir_tabuleiro(Estado estado)
{
  printf("Tabuleiro atual:\n");
  for (int i = 0; i < 3; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      printf("%c ", estado.tabuleiro[i * 3 + j]);
    }
    printf("\n");
  }
}

int main()
{
  printf("JOGO DA VELHA\n\n");

  do
  {
    printf("Quer ser X ou O?\n");
    scanf(" %c", &xisOuBola.min); // O espaço antes do %c é para limpar o buffer
    xisOuBola.min = tolower(xisOuBola.min);
    xisOuBola.max = (xisOuBola.min == 'x') ? 'o' : 'x';
  } while (xisOuBola.min != 'x' && xisOuBola.min != 'o');

  int vez_do_jogador;
  printf("Quer comecar? (S/n)\n");
  scanf(" %c", &vez_do_jogador); // O espaço antes do %c é para limpar o buffer
  vez_do_jogador = tolower(vez_do_jogador) == 'n' ? 0 : 1;

  // Inicializa o estado do jogo
  Estado estado = {{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}};

  exibir_tabuleiro(estado);

  while (!teste_terminal(estado))
  {
    if (!vez_do_jogador)
    {
      Acao acao = minimax_decision(estado);
      estado = resultado(estado, acao, 1);
      printf("Computador jogou: (%d, %d)\n", acao.linha + 1, acao.coluna + 1);
    }
    else
    {
      int linha, coluna, pos;
      do {
        printf("\nSua vez! Digite a linha e coluna (1-3): ");
        int inputs_lidos = scanf("%d %d", &linha, &coluna);
        
        if (inputs_lidos != 2) {
            // Limpa o buffer se o usuário digitou letras
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            linha = -1; // Força a repetição 
        }

        pos = (linha - 1) * 3 + (coluna - 1);
        
        if (linha < 1 || linha > 3 || coluna < 1 || coluna > 3 || estado.tabuleiro[pos] != ' ') {
            printf("Jogada invalida! Tente novamente.\n");
            pos = -1; // força a repetição do loop
        }
      } while(pos < 0);

      Acao acao = {linha - 1, coluna - 1}; // Ajusta para índice 0
      estado = resultado(estado, acao, 0);
    }
    exibir_tabuleiro(estado);
    vez_do_jogador = !vez_do_jogador; // Alterna entre MAX e MIN
  }

  // --- Resultado Final ---
  int resultado_final = utilidade(estado);
  if (resultado_final == 1)
    printf("\nO Computador venceu!\n");
  else if (resultado_final == -1)
    printf("\nVoce venceu!\n");
  else
    printf("\nDeu velha!\n");

  return 0;
}