#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <locale.h>
#include <string.h>
#include "matrizes_9.h"
#include "matrizes_10.h"

#define FREE 1
#define OBSTACLE 0
#define START 2
#define END 3

void zerarMatriz(int tamanhoMatriz, int **matriz)
{
  for (int i = 0; i < tamanhoMatriz; i++)
  {
    for (int j = 0; j < tamanhoMatriz; j++)
    {
      matriz[i][j] = 0;
    }
  }
}

void zerarVetor(int tamanhoVetor, int *vetor)
{
  for (int i = 0; i < tamanhoVetor; i++)
  {
    vetor[i] = 0;
  }
}

int nodes_visited = 0;
int nivel = 0;

typedef struct
{
  int x, y;
} Point;

typedef struct Node
{
  Point point;
  int g, h, f;
  struct Node *parent;
} Node;

Node *openList[81];
int openListSize = 0;

Node *closedList[81];
int closedListSize = 0;

void push(Node *node, Node **list, int *size)
{
  list[*size] = node;
  (*size)++;
  nodes_visited++;
}

void pop(Node *node, Node **list, int *size)
{
  for (int i = 0; i < *size; i++)
  {
    if (list[i] == node)
    {
      for (int j = i; j < *size - 1; j++)
      {
        list[j] = list[j + 1];
      }
      (*size)--;
      return;
    }
  }
}

Node *lowestFNode()
{
  Node *lowest = openList[0];
  for (int i = 1; i < openListSize; i++)
  {
    if (openList[i]->f < lowest->f)
    {
      lowest = openList[i];
    }
  }
  return lowest;
}

int heuristic(Point start, Point end)
{
  return abs(start.x - end.x) + abs(start.y - end.y);
}

Node *aStar(Point start, Point end, int **matriz)
{
  Node *startNode = (Node *)malloc(sizeof(Node));
  startNode->point = start;
  startNode->g = 0;
  startNode->h = heuristic(start, end);
  startNode->f = startNode->g + startNode->h;
  startNode->parent = NULL;

  push(startNode, openList, &openListSize);

  while (openListSize > 0)
  {
    Node *currentNode = lowestFNode();
    if (currentNode->point.x == end.x && currentNode->point.y == end.y)
    {
      return currentNode;
    }

    pop(currentNode, openList, &openListSize);
    push(currentNode, closedList, &closedListSize);

    for (int dx = -1; dx <= 1; dx++)
    {
      for (int dy = -1; dy <= 1; dy++)
      {
        if (dx == 0 && dy == 0)
        {
          continue;
        }

        int newX = currentNode->point.x + dx;
        int newY = currentNode->point.y + dy;

        if (newX >= 0 && newX < 9 && newY >= 0 && newY < 9 && matriz[newX][newY])
        {
          Node *neighborNode = (Node *)malloc(sizeof(Node));
          neighborNode->point.x = newX;
          neighborNode->point.y = newY;

          Node *neighborInClosedList = NULL;
          for (int i = 0; i < closedListSize; i++)
          {
            if (closedList[i]->point.x == newX && closedList[i]->point.y == newY)
            {
              neighborInClosedList = closedList[i];
              break;
            }
          }

          if (neighborInClosedList != NULL)
          {
            if (neighborInClosedList->g <= currentNode->g + 1)
            {
              free(neighborNode);
              continue;
            }
          }

          int g = currentNode->g + 1;
          int h = heuristic(neighborNode->point, end);
          neighborNode->g = g;
          neighborNode->h = h;
          neighborNode->f = g + h;
          neighborNode->parent = currentNode;

          Node *neighborInOpenList = NULL;
          for (int i = 0; i < openListSize; i++)
          {
            if (openList[i]->point.x == newX && openList[i]->point.y == newY)
            {
              neighborInOpenList = openList[i];
              break;
            }
          }

          if (neighborInOpenList == NULL)
          {
            push(neighborNode, openList, &openListSize);
          }
          else if (neighborInOpenList->g > neighborNode->g)
          {
            neighborInOpenList->g = neighborNode->g;
            neighborInOpenList->h = neighborNode->h;
            neighborInOpenList->f = neighborNode->f;
            neighborInOpenList->parent = currentNode;
          }
        }
      }
    }
  }

  return NULL;
}

typedef struct
{
  Point points[81]; // o tamanho máximo do caminho é 81 (9x9)
  int top;
} Stack;

void stack_init(Stack *stack)
{
  stack->top = -1;
}

void stack_push(Stack *stack, int x, int y)
{
  stack->top++;
  stack->points[stack->top].x = x;
  stack->points[stack->top].y = y;
}

void stack_pop(Stack *stack)
{
  stack->top--;
}

Point stack_top(Stack *stack)
{
  return stack->points[stack->top];
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

void encontrarPontoFinal(int *x, int *y, int tamanhoMatriz, int **matriz)
{
  for (int i = 0; i < tamanhoMatriz; i++)
  {
    for (int j = 0; j < tamanhoMatriz; j++)
    {
      if (matriz[i][j] == 3)
      {
        *x = i;
        *y = j;
        return;
      }
    }
  }
}

int dfs(int **matriz, int x, int y, Stack *path, int depth, int limit, int **visited)
{

  if (matriz[x][y] == OBSTACLE || depth > limit || visited[x][y])
  {
    return 0;
  }
  visited[x][y] = 1;
  nodes_visited++;
  if (matriz[x][y] == END)
  {
    stack_push(path, x, y);
    return 1;
  }
  matriz[x][y] = OBSTACLE;
  stack_push(path, x, y);
  if ((x > 0 && dfs(matriz, x - 1, y, path, depth + 1, limit, visited)) ||
      (x < 8 && dfs(matriz, x + 1, y, path, depth + 1, limit, visited)) ||
      (y > 0 && dfs(matriz, x, y - 1, path, depth + 1, limit, visited)) ||
      (y < 8 && dfs(matriz, x, y + 1, path, depth + 1, limit, visited)))
  {
    return 1;
  }
  matriz[x][y] = FREE; // desmarca a célula atual
  stack_pop(path);
  return 0;
}
int buscaProfundidadeIterativa(int tamanhoMatriz, int **matriz, Stack *path)
{
  int x, y;
  encontrarPontoPartida(&x, &y, tamanhoMatriz, matriz);
  for (int limit = 1;; limit++)
  {
    int **visitados = (int **)malloc(tamanhoMatriz * sizeof(int *));
    for (int i = 0; i < tamanhoMatriz; i++)
    {
      visitados[i] = malloc(tamanhoMatriz * sizeof(int));
    }
    zerarMatriz(tamanhoMatriz, visitados); // limpa a matriz visited
    nodes_visited = 0;                     // Zera o contador
    if (dfs(matriz, x, y, path, 0, limit, visitados))
    {
      printf("Número de nós visitados: %d\n", nodes_visited);
      printf("Caminho encontrado com limite %d:\n", limit);
      for (int k = 0; k <= path->top; k++)
      {
        printf("(%d, %d)\n", path->points[k].x, path->points[k].y);
      }
      return 0;
    }
  }
  printf("Caminho não encontrado.\n");
}

void buscaAstar(int tamanhoMatriz, int **matriz)
{
  Point start;
  Point end;
  int x, y;
  encontrarPontoPartida(&x, &y, tamanhoMatriz, matriz);
  start.x = x;
  start.y = y;
  encontrarPontoFinal(&x, &y, tamanhoMatriz, matriz);
  end.x = x;
  end.y = y;

  Node *endNode = aStar(start, end, matriz);

  if (endNode == NULL)
  {
    printf("Nenhum caminho encontrado.\n");
  }
  else
  {
    printf("Número de nós visitados: %d\n", nodes_visited);
    printf("Caminho encontrado:\n");
    Node *node = endNode;
    while (node != NULL)
    {
      printf("(%d, %d)\n", node->point.x, node->point.y);
      node = node->parent;
      nivel++;
    }
    printf("O caminho foi encotrado com profundidade %d/n", nivel);
  }
}

void adicionarAdjacencia(int **matriz, int i, int j, int valor)
{
  if (matriz[i][j] != END)
  {
    matriz[i][j] = valor;
  }
  if (matriz[j][i] != END)
  {
    matriz[j][i] = 1;
  }
}

int **gerarMatrizAdjacencias(int tamanhoMatriz, int **matrizLabirinto, int *verticeInicial)
{
  int novoTamanhoMatriz = tamanhoMatriz * tamanhoMatriz;

  int **matrizAdjacencias = (int **)malloc(novoTamanhoMatriz * sizeof(int *));
  for (int i = 0; i < novoTamanhoMatriz; i++)
  {
    matrizAdjacencias[i] = malloc(novoTamanhoMatriz * sizeof(int));
  }
  zerarMatriz(novoTamanhoMatriz, matrizAdjacencias);

  printf("\n\nGrafo resultante:\n");

  for (int i = 0; i < tamanhoMatriz; i++)
  {
    for (int j = 0; j < tamanhoMatriz; j++)
    {
      printf("%d (%d)\t", matrizLabirinto[i][j], i * tamanhoMatriz + j);

      // Se a célula não é uma parede
      // Verificar as células vizinhas
      if (matrizLabirinto[i][j] != OBSTACLE)
      {
        // Cima
        if (i > 0 && (matrizLabirinto[i - 1][j] == FREE || matrizLabirinto[i - 1][j] == START))
          adicionarAdjacencia(matrizAdjacencias, i * tamanhoMatriz + j, (i - 1) * tamanhoMatriz + j, 1);
        else if (i > 0 && matrizLabirinto[i - 1][j] == END)
          adicionarAdjacencia(matrizAdjacencias, i * tamanhoMatriz + j, (i - 1) * tamanhoMatriz + j, 3);

        // Baixo
        if (i < tamanhoMatriz - 1 && (matrizLabirinto[i + 1][j] == FREE || matrizLabirinto[i + 1][j] == START))
          adicionarAdjacencia(matrizAdjacencias, i * tamanhoMatriz + j, (i + 1) * tamanhoMatriz + j, 1);
        else if (i < tamanhoMatriz - 1 && matrizLabirinto[i + 1][j] == END)
          adicionarAdjacencia(matrizAdjacencias, i * tamanhoMatriz + j, (i + 1) * tamanhoMatriz + j, 3);

        // Esquerda
        if (j > 0 && (matrizLabirinto[i][j - 1] == FREE || matrizLabirinto[i][j - 1] == START))
          adicionarAdjacencia(matrizAdjacencias, i * tamanhoMatriz + j, i * tamanhoMatriz + (j - 1), 1);
        else if (j > 0 && matrizLabirinto[i][j - 1] == END)
          adicionarAdjacencia(matrizAdjacencias, i * tamanhoMatriz + j, i * tamanhoMatriz + (j - 1), 3);

        // Direita
        if (j < tamanhoMatriz - 1 && (matrizLabirinto[i][j + 1] == FREE || matrizLabirinto[i][j + 1] == START))
          adicionarAdjacencia(matrizAdjacencias, i * tamanhoMatriz + j, i * tamanhoMatriz + (j + 1), 1);
        else if (j < tamanhoMatriz - 1 && matrizLabirinto[i][j + 1] == END)
          adicionarAdjacencia(matrizAdjacencias, i * tamanhoMatriz + j, i * tamanhoMatriz + (j + 1), 3);

        if (matrizLabirinto[i][j] == START)
        {
          *verticeInicial = i * tamanhoMatriz + j;
        }
      }
    }
    printf("\n");
  }

  return matrizAdjacencias;
}

void buscaLargura(int tamanhoMatriz, int **matriz, int s)
{
  int qtdNosVisitados = 1;

  int *pilha = (int *)malloc(tamanhoMatriz * sizeof(int *));
  zerarVetor(tamanhoMatriz, pilha);

  int *vetorDistancias = (int *)malloc(tamanhoMatriz * sizeof(int *));
  zerarVetor(tamanhoMatriz, vetorDistancias);

  int front = 0, rear = -1, i;

  for (i = 0; i < tamanhoMatriz; i++)
    vetorDistancias[i] = -1;

  vetorDistancias[s] = 0;
  pilha[++rear] = s;

  int **caminhosVertices = (int **)malloc(tamanhoMatriz * sizeof(int *));
  for (int i = 0; i < tamanhoMatriz; i++)
  {
    caminhosVertices[i] = malloc(tamanhoMatriz * sizeof(int));
  }
  zerarMatriz(tamanhoMatriz, caminhosVertices);

  int *tamanhosCaminhosVertices = (int *)malloc(tamanhoMatriz * sizeof(int *));
  zerarVetor(tamanhoMatriz, tamanhosCaminhosVertices);

  caminhosVertices[s][tamanhosCaminhosVertices[s]++] = s;

  int verticeFinal = -1;

  while (front <= rear)
  {
    s = pilha[front++];
    for (i = 0; i < tamanhoMatriz; i++)
    {
      if (matriz[s][i] == END && vetorDistancias[i] == -1)
      {
        verticeFinal = i;
      }

      if (matriz[s][i] && vetorDistancias[i] == -1)
      {
        vetorDistancias[i] = vetorDistancias[s] + 1;
        pilha[++rear] = i;

        if (verticeFinal == -1)
          qtdNosVisitados++;

        for (int j = 0; j < tamanhosCaminhosVertices[s]; j++)
        {
          caminhosVertices[i][j] = caminhosVertices[s][j];
        }

        caminhosVertices[i][tamanhosCaminhosVertices[s]] = i;
        tamanhosCaminhosVertices[i] = tamanhosCaminhosVertices[s] + 1;
      }
    }
  }

  if (verticeFinal == -1)
  {
    printf("Caminho nao encontrado.\n");
  }
  else
  {
    printf("Vertice final encontrado: %d \n\n", verticeFinal);
    printf("Numero de nos visitados: %d\n", qtdNosVisitados);
    printf("Caminho encontrado: ");
    for (int j = 0; j < tamanhosCaminhosVertices[verticeFinal]; j++)
    {
      printf("(%d) ", caminhosVertices[verticeFinal][j]);
    }
    printf("\n");
    printf("O caminho foi encontrado com profundidade %d\n", vetorDistancias[verticeFinal]);
  }

  free(pilha);
  free(vetorDistancias);
  free(tamanhosCaminhosVertices);

  for (int i = 0; i < tamanhoMatriz; i++)
  {
    free(caminhosVertices[i]);
  }
  free(caminhosVertices);
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

  int tamanhoMatriz, opcaoLabirinto, opcaoAlgoritmo, verticeInicial;

  Stack path;
  stack_init(&path);
  do
  {
    printf("\n\n~~~~~~ Labirinto ~~~~~~\n\n");
    printf("Digite a dimensao do labirinto (8, 9 ou 10) ou 0 para sair:\n");
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
      case 2:;
        int **matrizAdjacencias = gerarMatrizAdjacencias(tamanhoMatriz, matrizSelecionada, &verticeInicial);
        printf("\nVertice inicial: %d\n", verticeInicial);
        // exibirMatriz(tamanhoMatriz * tamanhoMatriz, matrizAdjacencias);
        buscaLargura(tamanhoMatriz * tamanhoMatriz, matrizAdjacencias, verticeInicial);
        for (int i = 0; i < tamanhoMatriz; i++)
        {
          free(matrizAdjacencias[i]);
        }
        free(matrizAdjacencias);
        break;
      case 3:
        buscaAstar(tamanhoMatriz, matrizSelecionada);
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
    else if (tamanhoMatriz != 0)
    {
      printf("Tamanho de matriz nao valido, escolhe entre os tamanhos 8, 9 e 10/n");
    }

  } while (tamanhoMatriz != 0);

  return 0;
}
