#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <locale.h>
#include <string.h>
#include "matrizes_8.h"
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
int nivel = -1; // Variável inicia em -1 para desconsiderar o ponto de partida

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

Node *openList[100];
int openListSize = 0;

Node *closedList[100];
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

Node *aStar(Point start, Point end, int tamanhoMatriz, int **matriz)
{
  Node *startNode = (Node *)malloc(sizeof(Node));
  startNode->point = start;
  startNode->g = 0;
  startNode->h = heuristic(start, end);
  startNode->f = startNode->g + startNode->h;
  startNode->parent = NULL;

  openListSize = 0;
  closedListSize = 0;

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
        // Bloquear movimentos na diagonal
        // dx = 1 || dx = -1 -> dy = 0
        // dy = 1 || dy = -1 -> dx = 0
        if (abs(dx) == abs(dy))
        {
          continue;
        }

        int newX = currentNode->point.x + dx;
        int newY = currentNode->point.y + dy;

        if (newX >= 0 && newX < tamanhoMatriz && newY >= 0 && newY < tamanhoMatriz && matriz[newX][newY])
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
  Point *points; // o tamanho máximo do caminho é 81 (9x9)
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

void encontrarPontoPartida(int *x, int *y, int tamanhoMatriz, int **matriz)
{
  for (int i = 0; i < tamanhoMatriz; i++)
  {
    for (int j = 0; j < tamanhoMatriz; j++)
    {
      if (matriz[i][j] == START)
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
      if (matriz[i][j] == END)
      {
        *x = i;
        *y = j;
        return;
      }
    }
  }
}

int buscaProfundidade(int tamanhoMatriz, int **matriz, int x, int y, Stack *path, int nivel, int nivelMaximo, int **visitados)
{
  if (matriz[x][y] == OBSTACLE || nivel > nivelMaximo)
  {
    return 0;
  }
  if (visitados[x][y] != 1)
  {
    nodes_visited++;
    visitados[x][y] = 1;
  }

  if (matriz[x][y] == END)
  {
    stack_push(path, x, y);

    return 1;
  }
  matriz[x][y] = OBSTACLE;
  stack_push(path, x, y);
  if ((x > 0 && buscaProfundidade(tamanhoMatriz, matriz, x - 1, y, path, nivel + 1, nivelMaximo, visitados)) ||
      (x < tamanhoMatriz - 1 && buscaProfundidade(tamanhoMatriz, matriz, x + 1, y, path, nivel + 1, nivelMaximo, visitados)) ||
      (y > 0 && buscaProfundidade(tamanhoMatriz, matriz, x, y - 1, path, nivel + 1, nivelMaximo, visitados)) ||
      (y < tamanhoMatriz - 1 && buscaProfundidade(tamanhoMatriz, matriz, x, y + 1, path, nivel + 1, nivelMaximo, visitados)))
  {
    return 1;
  }
  matriz[x][y] = FREE; // desmarca a célula atual
  stack_pop(path);

  return 0;
}

void buscaProfundidadeIterativa(int tamanhoMatriz, int **matrizSelecionada)
{
  Stack *path = (Stack *)malloc(sizeof(Stack));
  path->points = malloc((tamanhoMatriz * tamanhoMatriz * sizeof(Point *)));

  stack_init(path);

  int **matriz = (int **)malloc(tamanhoMatriz * sizeof(int *));
  for (int i = 0; i < tamanhoMatriz; i++)
  {
    matriz[i] = malloc(tamanhoMatriz * sizeof(int));
  }

  for (int i = 0; i < tamanhoMatriz; i++)
    for (int j = 0; j < tamanhoMatriz; j++)
      matriz[i][j] = matrizSelecionada[i][j];

  int x, y;
  encontrarPontoPartida(&x, &y, tamanhoMatriz, matriz);

  for (int limit = 1; limit < tamanhoMatriz * tamanhoMatriz; limit++)
  {
    int **visitados = (int **)malloc(tamanhoMatriz * sizeof(int *));
    for (int i = 0; i < tamanhoMatriz; i++)
    {
      visitados[i] = malloc(tamanhoMatriz * sizeof(int));
    }
    zerarMatriz(tamanhoMatriz, visitados);
    nodes_visited = 0; // Zera o contador

    if (buscaProfundidade(tamanhoMatriz, matriz, x, y, path, 0, limit, visitados))
    {
      printf("Número de nós visitados: %d\n", nodes_visited);
      printf("Caminho encontrado com limite %d:\n", limit);
      for (int k = 0; k <= path->top; k++)
      {
        printf("(%d, %d)\n", path->points[k].x, path->points[k].y);
      }

      for (int i = 0; i < tamanhoMatriz; i++)
      {
        free(visitados[i]);
      }
      free(visitados);
      free(path->points);
      free(path);
      for (int i = 0; i < tamanhoMatriz; i++)
      {
        free(matriz[i]);
      }
      free(matriz);

      return;
    }

    for (int i = 0; i < tamanhoMatriz; i++)
    {
      free(visitados[i]);
    }
    free(visitados);
  }

  printf("Caminho não encontrado.\n");

  free(path->points);
  free(path);
  for (int i = 0; i < tamanhoMatriz; i++)
  {
    free(matriz[i]);
  }
  free(matriz);
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

  nodes_visited = 0;
  nivel = -1;

  Node *endNode = aStar(start, end, tamanhoMatriz, matriz);

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

void buscaLargura(int tamanhoMatrizLabirinto, int **matrizLabirinto)
{
  int verticeAtual;
  int **matrizAdjacencias = gerarMatrizAdjacencias(tamanhoMatrizLabirinto, matrizLabirinto, &verticeAtual);
  int tamanhoMatriz = tamanhoMatrizLabirinto * tamanhoMatrizLabirinto;
  printf("\nVertice inicial: %d\n", verticeAtual);

  int qtdNosVisitados = 1;

  int *pilha = (int *)malloc(tamanhoMatriz * sizeof(int *));
  zerarVetor(tamanhoMatriz, pilha);

  int *vetorDistancias = (int *)malloc(tamanhoMatriz * sizeof(int *));
  zerarVetor(tamanhoMatriz, vetorDistancias);

  int front = 0, rear = -1, i;

  for (i = 0; i < tamanhoMatriz; i++)
    vetorDistancias[i] = -1;

  vetorDistancias[verticeAtual] = 0;
  pilha[++rear] = verticeAtual;

  int **caminhosVertices = (int **)malloc(tamanhoMatriz * sizeof(int *));
  for (int i = 0; i < tamanhoMatriz; i++)
  {
    caminhosVertices[i] = malloc(tamanhoMatriz * sizeof(int));
  }
  zerarMatriz(tamanhoMatriz, caminhosVertices);

  int *tamanhosCaminhosVertices = (int *)malloc(tamanhoMatriz * sizeof(int *));
  zerarVetor(tamanhoMatriz, tamanhosCaminhosVertices);

  caminhosVertices[verticeAtual][tamanhosCaminhosVertices[verticeAtual]++] = verticeAtual;

  int verticeFinal = -1;

  while (front <= rear)
  {
    verticeAtual = pilha[front++];
    for (i = 0; i < tamanhoMatriz; i++)
    {
      if (matrizAdjacencias[verticeAtual][i] == END && vetorDistancias[i] == -1)
      {
        verticeFinal = i;
      }

      if (matrizAdjacencias[verticeAtual][i] && vetorDistancias[i] == -1)
      {
        vetorDistancias[i] = vetorDistancias[verticeAtual] + 1;
        pilha[++rear] = i;

        if (verticeFinal == -1)
          qtdNosVisitados++;

        for (int j = 0; j < tamanhosCaminhosVertices[verticeAtual]; j++)
        {
          caminhosVertices[i][j] = caminhosVertices[verticeAtual][j];
        }

        caminhosVertices[i][tamanhosCaminhosVertices[verticeAtual]] = i;
        tamanhosCaminhosVertices[i] = tamanhosCaminhosVertices[verticeAtual] + 1;
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

  for (int i = 0; i < tamanhoMatriz; i++)
  {
    free(matrizAdjacencias[i]);
  }
  free(matrizAdjacencias);
}

/// Funcoes para Subida de Encosta
int contarVizinhosLivres(Point atual, int **matriz, int tamanhoMatriz)
{
  int livres = 0;
  int vizinho;

  vizinho = atual.x - 1;

  if (vizinho >= 0)
    if (matriz[vizinho][atual.y] == 1)
      livres += 1;

  vizinho = atual.x + 1;

  if (vizinho < tamanhoMatriz)
    if (matriz[vizinho][atual.y] == 1)
      livres += 1;

  vizinho = atual.y - 1;

  if (vizinho >= 0)
    if (matriz[atual.x][vizinho] == 1)
      livres += 1;

  vizinho = atual.y + 1;

  if (vizinho < tamanhoMatriz)
    if (matriz[atual.x][vizinho] == 1)
      livres += 1;

  return livres;
}

double avaliacao(Point atual, Point destino, int **matriz, int tamanhoMatriz)
{
  int distancia = abs(destino.x - atual.x) + abs(destino.y - atual.y);
  int livres = contarVizinhosLivres(atual, matriz, tamanhoMatriz);

  if (atual.x == destino.x && atual.y == destino.y)
    return -10; // Para sempre escolher a chegada no objetivo

  if (livres != 0)
    return distancia - livres;

  return distancia + 10; // Penalização por um caminho sem saida
}

void subidaEncosta(int tamanhoMatriz, int **matriz)
{
  Point inicio;
  Point fim;
  int x, y;
  encontrarPontoPartida(&x, &y, tamanhoMatriz, matriz);
  inicio.x = x;
  inicio.y = y;
  encontrarPontoFinal(&x, &y, tamanhoMatriz, matriz);
  fim.x = x;
  fim.y = y;

  Point atual = inicio;

  Point passados[20]; // Talvez  colocar uma fila com alocação dinamica depois
  passados[0] = inicio;
  int fim_passados = 1;

  while (atual.x != fim.x || atual.y != fim.y)
  {
    Point verificado = atual;
    Point melhor = verificado;
    int melhor_valor = avaliacao(verificado, fim, matriz, tamanhoMatriz);

    int valor_verificado;

    // Vertical de cima para baixo
    for (int i = atual.y; i < tamanhoMatriz; i++)
    {
      verificado.y = i;
      if (matriz[verificado.x][verificado.y] == 0)
      {        // Verifica se o movimento seria válido
        break; // Não é possivel passar por aqui
      }
      valor_verificado = avaliacao(verificado, fim, matriz, tamanhoMatriz);
      if (valor_verificado < melhor_valor)
      {
        // printf("Mudou o melhor de (%d, %d) para (%d, %d)", melhor.x, melhor.y, verificado.x, verificado.y);
        melhor_valor = valor_verificado;
        melhor = verificado;
      }
      // printf("CB: Ponto estudado =  (%d, %d), com avaliacao: %d, i = %d, tamanho = %d\n", verificado.x, verificado.y, valor_verificado, i, tamanhoMatriz);
    }

    // Vertical de baixo pra cima
    for (int i = atual.y - 1; i >= 0; i--)
    {
      verificado.y = i;
      if (matriz[verificado.x][verificado.y] == 0) // Verifica se o movimento seria válido
        break;                                     // Não é possivel passar por aqui
      valor_verificado = avaliacao(verificado, fim, matriz, tamanhoMatriz);
      if (valor_verificado < melhor_valor)
      {
        // printf("Mudou o melhor de (%d, %d) para (%d, %d)", melhor.x, melhor.y, verificado.x, verificado.y);
        melhor_valor = valor_verificado;
        melhor = verificado;
      }
      // printf("BC: Ponto estudado =  (%d, %d), com avaliacao: %d, i = %d, tamanho = %d\n", verificado.x, verificado.y, valor_verificado, i, tamanhoMatriz);
    }

    // Adiciona a casa escolhida na vertical
    atual = melhor; /// Escolhe a melhor casa da vertical para andar
    passados[fim_passados] = melhor;
    fim_passados += 1;
    verificado = melhor;

    // Horizontal esquerda para direita
    for (int i = atual.x; i < tamanhoMatriz; i++)
    {
      verificado.x = i;
      if (matriz[verificado.x][verificado.y] == 0) // Verifica se o movimento seria válido
        break;                                     // Não é possivel passar por aqui
      valor_verificado = avaliacao(verificado, fim, matriz, tamanhoMatriz);
      if (valor_verificado < melhor_valor)
      {
        // printf("Mudou o melhor de (%d, %d) para (%d, %d)", melhor.x, melhor.y, verificado.x, verificado.y);
        melhor_valor = valor_verificado;
        melhor = verificado;
      }
      // printf("ED: Ponto estudado =  (%d, %d), com avaliacao: %d, i = %d, tamanho = %d\n", verificado.x, verificado.y, valor_verificado, i, tamanhoMatriz);
    }

    // Horizontal da direita pra esquerda
    for (int i = atual.x - 1; i >= 0; i--)
    {
      verificado.x = i;
      if (matriz[verificado.x][verificado.y] == 0) // Verifica se o movimento seria válido
        break;                                     // Não é possivel passar por aqui
      valor_verificado = avaliacao(verificado, fim, matriz, tamanhoMatriz);
      if (valor_verificado < melhor_valor)
      {
        // printf("Mudou o melhor de (%d, %d) para (%d, %d)", melhor.x, melhor.y, verificado.x, verificado.y);
        melhor_valor = valor_verificado;
        melhor = verificado;
      }
      // printf("DE: Ponto estudado =  (%d, %d), com avaliacao: %d, i = %d, tamanho = %d\n", verificado.x, verificado.y, valor_verificado, i, tamanhoMatriz);
    }

    atual = melhor; /// Escolhe a melhor casa da vertical para andar
    passados[fim_passados] = melhor;
    fim_passados += 1;
  }

  // Print do caminho passado pelo robo
  if (atual.x == fim.x && atual.y == fim.y)
  {
    printf("O robo passou por: (%d, %d) -> ", passados[0].x, passados[0].y);
    for (int i = 1; i < fim_passados - 1; i++)
    {
      printf("(%d, %d) -> ", passados[i].x, passados[i].y);
    }
    printf("(%d, %d). Para chegar no objetivo.", passados[fim_passados - 1].x, passados[fim_passados - 1].y);
  }
  else
  {
    printf("Não chegamos no objetivo, chegamos eim: (%d,%d)", atual.x, atual.y);
  }
}

/// --------------------------------------------------------------------------

void exibirOpcoesLabirinto(int tamanhoMatriz)
{
  switch (tamanhoMatriz)
  {
  case 8:
    printf("\n\nOpcao 1:\n");
    exibirMatrizFixa(tamanhoMatriz, matriz_8_1);

    printf("\n\nOpcao 2:\n");
    exibirMatrizFixa(tamanhoMatriz, matriz_8_2);

    printf("\n\nOpcao 3:\n");
    exibirMatrizFixa(tamanhoMatriz, matriz_8_3);

    printf("\n\nOpcao 4:\n");
    exibirMatrizFixa(tamanhoMatriz, matriz_8_4);

    printf("\n\nOpcao 5:\n");
    exibirMatrizFixa(tamanhoMatriz, matriz_8_5);
    break;

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
  case 8:
    switch (opcaoLabirinto)
    {
    case 1:
      return gerarMatriz(tamanhoMatriz, matriz_8_1);
      break;
    case 2:
      return gerarMatriz(tamanhoMatriz, matriz_8_2);
      break;
    case 3:
      return gerarMatriz(tamanhoMatriz, matriz_8_3);
      break;
    case 4:
      return gerarMatriz(tamanhoMatriz, matriz_8_4);
      break;
    case 5:
      return gerarMatriz(tamanhoMatriz, matriz_8_5);
      break;
    default:
      return gerarMatriz(tamanhoMatriz, matriz_8_1);
      break;
    }
    break;

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
        buscaProfundidadeIterativa(tamanhoMatriz, matrizSelecionada);
        break;
      case 2:
        buscaLargura(tamanhoMatriz, matrizSelecionada);
        break;
      case 3:
        buscaAstar(tamanhoMatriz, matrizSelecionada);
        break;
      case 4:
        subidaEncosta(tamanhoMatriz, matrizSelecionada);
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
