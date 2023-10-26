#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <locale.h>
#include "matrizes_9.h"
#include "matrizes_10.h"

#define FREE 1
#define OBSTACLE 0
#define START 2
#define END 3

void zerarVisitados(int tamanhoMatriz, int **visitados)
{
  for (int i = 0; i < tamanhoMatriz; i++)
  {
    for (int j = 0; j < tamanhoMatriz; j++)
    {
      visitados[i][j] = 0;
    }
  }
}


typedef struct {
    int x, y;
} Point;

typedef struct {
    Point points[81]; // o tamanho máximo do caminho é 81 (9x9)
    int top;
} Stack;

void stack_init(Stack *stack) {
    stack->top = -1;
}

void stack_push(Stack *stack, int x, int y) {
    stack->top++;
    stack->points[stack->top].x = x;
    stack->points[stack->top].y = y;
}

void stack_pop(Stack *stack) {
    stack->top--;
}

Point stack_top(Stack *stack) {
    return stack->points[stack->top];
}

int nodes_visited = 0;

void exibirMatrizFixa(int tamanhoMatriz, int matriz[tamanhoMatriz][tamanhoMatriz])
{
  int i, j;
  for (i = 0; i < tamanhoMatriz; i++)
  {
    for (j = 0; j < tamanhoMatriz; j++)
    {
      printf("%d ", matriz[i][j]);
    }
    printf("\n");
  }
}

void exibirMatriz(int tamanhoMatriz, int **matriz)
{
  int i, j;
  for (i = 0; i < tamanhoMatriz; i++)
  {
    for (j = 0; j < tamanhoMatriz; j++)
    {
      printf("%d ", matriz[i][j]);
    }
    printf("\n");
  }
}

int encontrouSaida(int x, int y, int tamanhoMatriz, int **matriz)
{
  return x >= 0 && x < tamanhoMatriz && y >= 0 && y < tamanhoMatriz && matriz[x][y] == 3;
}


void encontrarPontoPartida(int *x, int *y, int tamanhoMatriz, int **matriz)
{
  for (int i = 0; i < tamanhoMatriz; i++)
  {
    for (int j = 0; j < tamanhoMatriz; j++)
    {
      if (matriz[i][j] == 2)
      {
        *x = i;
        *y = j;
        return;
      }
    }
  }
}

int dfs( int **matriz, int x, int y, Stack *path, int depth, int limit, int **visited) {
    
    if (matriz[x][y] == OBSTACLE || depth > limit || visited[x][y]) {
        return 0;
    }
    visited[x][y] = 1;
    nodes_visited++;
    if (matriz[x][y] == END) {
        stack_push(path, x, y);
        return 1;
    }
    matriz[x][y] = OBSTACLE;
    stack_push(path, x, y);
    if ((x > 0 && dfs(matriz, x-1, y, path, depth+1, limit, visited)) ||
        (x < 8 && dfs(matriz, x+1, y, path, depth+1, limit, visited)) ||
        (y > 0 && dfs(matriz, x, y-1, path, depth+1, limit, visited)) ||
        (y < 8 && dfs(matriz, x, y+1, path, depth+1, limit, visited))) {
        return 1;
    }
    matriz[x][y] = FREE; // desmarca a célula atual
    stack_pop(path);
    return 0;
}
int buscaProfundidadeIterativa(int tamanhoMatriz, int **matriz, Stack *path){
    int x, y;
    encontrarPontoPartida(&x, &y, tamanhoMatriz, matriz);
    for (int limit = 1; ; limit++) {
        int **visitados = (int **)malloc(tamanhoMatriz * sizeof(int *));
        for (int i = 0; i < tamanhoMatriz; i++)
        {
          visitados[i] = malloc(tamanhoMatriz * sizeof(int));
        }
        zerarVisitados(tamanhoMatriz, visitados); // limpa a matriz visited
        nodes_visited = 0; // Zera o contador
                        if (dfs(matriz, x, y, path, 0, limit, visitados)) {
                            printf("Número de nós visitados: %d\n", nodes_visited);
                            printf("Caminho encontrado com limite %d:\n", limit);
                            for (int k = 0; k <= path->top; k++) {
                                printf("(%d, %d)\n", path->points[k].x, path->points[k].y);
                            }
                            return 0;
                        }
            }
        printf("Caminho não encontrado.\n");
}

void exibirOpcoesLabirinto(int tamanhoMatriz)
{
  switch (tamanhoMatriz)
  {
  case 9:
    printf("\n\nOpcao 1:\n");
    exibirMatrizFixa(tamanhoMatriz, matriz_1);

    printf("\n\nOpcao 2:\n");
    exibirMatrizFixa(tamanhoMatriz, matriz_2);

    printf("\n\nOpcao 3:\n");
    exibirMatrizFixa(tamanhoMatriz, matriz_3);

    printf("\n\nOpcao 4:\n");
    exibirMatrizFixa(tamanhoMatriz, matriz_4);

    printf("\n\nOpcao 5:\n");
    exibirMatrizFixa(tamanhoMatriz, matriz_5);
    break;

  case 10:
    printf("\n\nOpcao 1:\n");
    exibirMatrizFixa(tamanhoMatriz, matriz_10_1);

    printf("\n\nOpcao 2:\n");
    exibirMatrizFixa(tamanhoMatriz, matriz_10_2);

    printf("\n\nOpcao 3:\n");
    exibirMatrizFixa(tamanhoMatriz, matriz_10_3);

    printf("\n\nOpcao 4:\n");
    exibirMatrizFixa(tamanhoMatriz, matriz_10_4);

    printf("\n\nOpcao 5:\n");
    exibirMatrizFixa(tamanhoMatriz, matriz_10_5);
    break;

  default:
    break;
  }
}

int **gerarMatriz(int tamanhoMatriz, int matriz[tamanhoMatriz][tamanhoMatriz])
{
  int **novaMatriz = (int **)malloc(tamanhoMatriz * sizeof(int *));
  for (int i = 0; i < tamanhoMatriz; i++)
  {
    novaMatriz[i] = malloc(tamanhoMatriz * sizeof(int));
  }

  for (int i = 0; i < tamanhoMatriz; i++)
  {
    novaMatriz[i] = matriz[i];
  }

  return novaMatriz;
}

int **selecionarOpcao(int tamanhoMatriz, int opcaoLabirinto)
{
  switch (tamanhoMatriz)
  {
  case 9:
    switch (opcaoLabirinto)
    {
    case 1:
      return gerarMatriz(tamanhoMatriz, matriz_1);
      break;
    case 2:
      return gerarMatriz(tamanhoMatriz, matriz_2);
      break;
    case 3:
      return gerarMatriz(tamanhoMatriz, matriz_3);
      break;
    case 4:
      return gerarMatriz(tamanhoMatriz, matriz_4);
      break;
    case 5:
      return gerarMatriz(tamanhoMatriz, matriz_5);
      break;
    default:
      return gerarMatriz(tamanhoMatriz, matriz_1);
      break;
    }
    break;

  case 10:
    switch (opcaoLabirinto)
    {
    case 1:
      return gerarMatriz(tamanhoMatriz, matriz_10_1);
      break;
    case 2:
      return gerarMatriz(tamanhoMatriz, matriz_10_2);
      break;
    case 3:
      return gerarMatriz(tamanhoMatriz, matriz_10_3);
      break;
    case 4:
      return gerarMatriz(tamanhoMatriz, matriz_10_4);
      break;
    case 5:
      return gerarMatriz(tamanhoMatriz, matriz_10_5);
      break;
    default:
      return gerarMatriz(tamanhoMatriz, matriz_10_1);
      break;
    }
    break;

  default:
    return gerarMatriz(tamanhoMatriz, matriz_1);
    break;
  }
}

int main(void)
{
  setlocale(LC_ALL, "Portuguese");

  int tamanhoMatriz, opcaoLabirinto, opcaoAlgoritmo;
    Stack path;
    stack_init(&path);
  do
  {
    printf("\n\n~~~~~~ Labirinto ~~~~~~\n\n");
    printf("Digite a dimensao do labirinto ( 8, 9 ou 10) ou 0 para sair:\n");
    scanf("%d", &tamanhoMatriz);

    if (tamanhoMatriz == 8 || tamanhoMatriz == 9 || tamanhoMatriz == 10)
    {
      printf("\nEscolha um labirinto:");
      exibirOpcoesLabirinto(tamanhoMatriz);

      printf("Digite a opcao desejada:\n");
      scanf("%d", &opcaoLabirinto);
      int **matrizSelecionada = selecionarOpcao(tamanhoMatriz, opcaoLabirinto);

      printf("\nEscolha um algoritmo:\n1 - Busca em Profundidade Iterativa\n2 - Busca de Menor Custo\n3 - Busca A*\n4 - Subida de Encosta\n\n");
      scanf("%d", &opcaoAlgoritmo);

      switch (opcaoAlgoritmo)
      {
      case 1:
        buscaProfundidadeIterativa(tamanhoMatriz, matrizSelecionada, &path);
        break;
      default:
        break;
      }

      for (int i = 0; i < tamanhoMatriz; i++)
      {
        free(matrizSelecionada[i]);
      }
      free(matrizSelecionada);
    }
    else {
        printf("Tamanho de matriz não válido, escolhe entre os tamanhos 8, 9 e 10/n");
    }

  } while (tamanhoMatriz != 0);

  return 0;
}
