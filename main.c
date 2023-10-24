#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <locale.h>
#include "matrizes_9.h"

#define MAX_SIZE 9

int visited[MAX_SIZE][MAX_SIZE];
int stack[MAX_SIZE * MAX_SIZE];
int top = -1;

void zerarVisitados()
{
  for (int i = 0; i < 9; i++)
  {
    for (int j = 0; j < 9; j++)
    {
      visited[i][j] = 0;
    }
  }
}

void esvaziarPilha()
{
  while (top != -1)
  {
    int nx, ny;
    pop(&nx, &ny);
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

int is_valid(int x, int y, int matriz[9][9])
{
  return x >= 0 && x < MAX_SIZE && y >= 0 && y < MAX_SIZE && matriz[x][y] == 1 && visited[x][y] != 1;
}

bool dfs(int x, int y, int matriz[9][9], int lMax)
{
  printf("DFS X = %d Y = %d\n", x, y);
  int l = 1;
  visited[x][y] = 1;
  push(x, y);

  while (top != -1 && l <= lMax)
  {
    // printf("Nivel local l = %d\n", l);

    int nx, ny;
    pop(&nx, &ny);

    // printf("X = %d Y = %d\n", nx, ny);

    int dx[] = {-1, 0, 1, 0};
    int dy[] = {0, 1, 0, -1};

    for (int i = 0; i < 4; i++)
    {
      int nnx = nx + dx[i]; // -1
      int nny = ny + dy[i]; // 3

      // printf("nnx = %d nny = %d\n", nnx, nny);
      if (matriz[nnx][nny] == 3)
      {
        printf("Achou X = %d Y = %d\n", nnx, nny);
        return true;
      }
      else if (is_valid(nnx, nny, matriz))
      {
        printf("Visitado X = %d Y = %d\n", nnx, nny);
        visited[nnx][nny] = 1;
        push(nnx, nny);
        l++;
      }
    }
  }

  return false;
}

void dfs_iterativa(int x, int y, int matriz[9][9])
{
  int lMax = 1;
  zerarVisitados();
  esvaziarPilha();

  while (!dfs(x, y, matriz, lMax))
  {
    esvaziarPilha();
    zerarVisitados();
    lMax++;
    printf("Nivel maximo lMax = %d\n", lMax);
  }
}

void exibirMatriz_9x9(int matriz[9][9])
{
  int i, j;
  for (i = 0; i < 9; i++)
  {
    for (j = 0; j < 9; j++)
    {
      printf("%d ", matriz[i][j]);
    }
    printf("\n");
  }
}

void exibirOpcoesLabirinto(int tamanho)
{
  setlocale(LC_ALL, "Portuguese");

  switch (tamanho)
  {
  case 9:
    printf("\n\nOpcao 1:\n");
    exibirMatriz_9x9(matriz_1);
    printf("\n\nOpcao 2:\n");
    exibirMatriz_9x9(matriz_2);
    printf("\n\nOpcao 3:\n");
    exibirMatriz_9x9(matriz_3);
    printf("\n\nOpcao 4:\n");
    exibirMatriz_9x9(matriz_4);
    printf("\n\nOpcao 5:\n");
    exibirMatriz_9x9(matriz_5);
    break;
  default:
    break;
  }
}

int main(void)
{
  setlocale(LC_ALL, "Portuguese");

  int tamanho, opcaoLabirinto, opcaoAlgoritmo;
  do
  {
    zerarVisitados();

    printf("Digite o tamanho do labirinto:\n8 -> 8x8\n9 -> 9x9\n10 -> 10x10\n");
    scanf("%d", &tamanho);

    printf("Escolha um labirinto:\n");
    exibirOpcoesLabirinto(tamanho);
    printf("Digite a opcao desejada:\n");
    scanf("%d", &opcaoLabirinto);
    printf("Escolha um algoritmo:\n1 - Busca em Profundidade Iterativa\n2 - Busca de Menor Custo\n3 - Busca A*\n4 - Subida de Encosta\n\n");
    scanf("%d", &opcaoAlgoritmo);

    switch (opcaoAlgoritmo)
    {
    case 1:
      dfs_iterativa(3, 0, matriz_1);
      break;
    default:
      break;
    }

  } while (opcaoLabirinto != 0);

  return 0;
}
