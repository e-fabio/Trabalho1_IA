#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <locale.h>
#include "matrizes_9.h"
#include "matrizes_10.h"

#define MAX_SIZE 10

int stack[MAX_SIZE * MAX_SIZE];
int top = -1;

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

void push(int x, int y)
{
  stack[++top] = x;
  stack[++top] = y;
}

void pop(int *x, int *y)
{
  *y = stack[top--];
  *x = stack[top--];
}

void esvaziarPilha()
{
  while (top != -1)
  {
    int nx, ny;
    pop(&nx, &ny);
  }
}

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

int movimentoValido(int x, int y, int tamanhoMatriz, int **matriz, int **visitados)
{
  return x >= 0 && x < tamanhoMatriz && y >= 0 && y < tamanhoMatriz && matriz[x][y] == 1 && visitados[x][y] == 0;
}

bool buscaProfundidade(int x, int y, int tamanhoMatriz, int **matriz, int **visitados, int lMax)
{
  printf("\n~~~~~~ Iniciando busca em profundidade iterativa:  X = %d, Y = %d, Nivel Maximo = %d ~~~~~~\n", x, y, lMax);
  int l = 1;

  visitados[x][y] = 1;

  push(x, y);

  while (top != -1 && l <= lMax)
  {
    printf("Nivel %d\n", l);

    int nx, ny;
    pop(&nx, &ny);
    bool aumentaNivel = true;

    int dx[] = {-1, 0, 1, 0};
    int dy[] = {0, 1, 0, -1};

    for (int i = 0; i < 4; i++)
    {
      int nnx = nx + dx[i];
      int nny = ny + dy[i];

      if (encontrouSaida(nnx, nny, tamanhoMatriz, matriz))
      {
        printf("Visitado X = %d Y = %d\n", nnx, nny);
        visitados[nnx][nny] = 1;
        exibirMatriz(tamanhoMatriz, visitados);
        printf("~~~~~~ Achou X = %d Y = %d ~~~~~~\n", nnx, nny);

        return true;
      }
      else if (movimentoValido(nnx, nny, tamanhoMatriz, matriz, visitados))
      {
        printf("Visitado X = %d Y = %d\n", nnx, nny);
        visitados[nnx][nny] = 1;
        exibirMatriz(tamanhoMatriz, visitados);
        // exibirMatriz(tamanhoMatriz, matriz);
        push(nnx, nny);

        if (aumentaNivel == true)
        {
          ++l;
          aumentaNivel = false;
        }
      }
    }
  }

  printf("~~~~~~ Saida nao encontrada ~~~~~~\n");

  return false;
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

void buscaProfundidadeIterativa(int tamanhoMatriz, int **matriz)
{
  int lMax = 1;

  int **visitados = (int **)malloc(tamanhoMatriz * sizeof(int *));
  for (int i = 0; i < tamanhoMatriz; i++)
  {
    visitados[i] = malloc(tamanhoMatriz * sizeof(int));
  }
  zerarVisitados(tamanhoMatriz, visitados);
  esvaziarPilha();

  int x, y;
  encontrarPontoPartida(&x, &y, tamanhoMatriz, matriz);

  while (!buscaProfundidade(x, y, tamanhoMatriz, matriz, visitados, lMax))
  {
    zerarVisitados(tamanhoMatriz, visitados);
    esvaziarPilha();
    ++lMax;
  }

  for (int i = 0; i < tamanhoMatriz; i++)
  {
    free(visitados[i]);
  }
  free(visitados);
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
  do
  {
    printf("\n\n~~~~~~ Labirinto ~~~~~~\n\n");
    printf("Digite a dimensao do labirinto ou 0 para sair:\n");
    scanf("%d", &tamanhoMatriz);

    if (tamanhoMatriz != 0)
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
        buscaProfundidadeIterativa(tamanhoMatriz, matrizSelecionada);
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

  } while (tamanhoMatriz != 0);

  return 0;
}
