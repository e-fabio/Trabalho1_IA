#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include "matrizes_9.h"

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
    printf("\n\nOpção 1:\n");
    exibirMatriz_9x9(matriz_1);
    printf("\n\nOpção 2:\n");
    exibirMatriz_9x9(matriz_2);
    printf("\n\nOpção 3:\n");
    exibirMatriz_9x9(matriz_3);
    printf("\n\nOpção 4:\n");
    exibirMatriz_9x9(matriz_4);
    printf("\n\nOpção 5:\n");
    exibirMatriz_9x9(matriz_5);
    break;
  default:
    break;
  }
}

int main(void)
{
  setlocale(LC_ALL, "Portuguese");

  int tamanho, opcaoLabirinto;
  do
  {
    printf("Digite o tamanho do labirinto:\n8 -> 8x8\n9 -> 9x9\n10 -> 10x10\n");
    scanf("%d", &tamanho);

    printf("Escolha um labirinto:\n");
    exibirOpcoesLabirinto(tamanho);
    printf("Digite a opção desejada:\n");
    scanf("%d", &opcaoLabirinto);

  } while (opcaoLabirinto != 0);

  return 0;
}
