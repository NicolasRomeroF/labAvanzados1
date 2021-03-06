#include <stdio.h>
#include <stdlib.h>
#include "structs.h"
#include "funciones.h"

//Posiciones para recorrer el puzzle
int dx[4] = {0,0,-1,1};
int dy[4] = {-1,1,0,0};

//Nodos que indican el inicio el final de la cola
struct Node* start = NULL;
struct Node* end = NULL;

//Nodos que marcan el inicio y el final de la lista de visitados
struct Node* visited =NULL;
struct Node* visitedEnd =NULL;

//Funcion que encola un puzzle junto a su profundidad
void enqueue(char** puzzle, int depth)
{
	Node* node = (Node*)malloc(sizeof(Node));
	node->puzzle = puzzle;
	node->depth = depth;
	node->next = NULL;

	if(start == NULL && end == NULL)
	{
		start = end = node;
		return;
	}
	end->next = node;
	end = node;

}

//Funcion que añade un puzzle a lista de visitados
void addVisited(char** puzzle)
{
	Node* node = (Node*)malloc(sizeof(Node));
	node->puzzle = puzzle;
	node->depth = -1;
	node->next = NULL;

	if(visited == NULL && visitedEnd == NULL)
	{
		visited = visitedEnd = node;
		return;
	}
	visitedEnd->next = node;
	visitedEnd = node;


}

//Funcion que desencola un elemento de la cola y lo retorna
//Salida: Nodo
Node* dequeue()
{
	Node* node = start;
	if(start == NULL)
	{
		return NULL;
	}
	if(start==end)
	{
		start = NULL;
		end = NULL;
	}
	else
	{
		start = start->next;
	}
	return node;
}

//Funcion que libera la memoria ocupada por una matriz (puzzle)
void freeMatrix(char** matrix)
{
	int i;
	for(i=0;i<3;i++)
	{
		free(matrix[i]);
	}
	free(matrix);
}

//Funcion que libera el espacio ocupado por un nodo
void freeNode(Node* n)
{
	//freeMatrix(n->puzzle);
	free(n);
}

//Funcion que asigna espacio para una matriz de NxM
char** createMatrix(int N, int M)
{
	char** matriz = (char**)malloc(sizeof(char*)*N);
	int i, j;
	if (matriz == NULL)
	{
		printf("ERROR: No se pudo asignar memoria a la matriz\n");
		return NULL;
	}
	for (i = 0; i < N; i++)
	{
		matriz[i] = (char*)malloc(sizeof(char) * M);
		if (matriz[i] == NULL)
		{
			printf("ERROR: No se pudo asignar memoria a la matriz\n");
			return NULL;
		}
	}
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < M; j++)
		{
			matriz[i][j] = ' ';
		}
	}
	return matriz;
}

//Funcion que muestra por pantalla una matriz (puzzle)
void printMatrix(char** matriz, int N, int M)
{
	printf("\nInicio Matriz\n\n");
	int i, j;
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < M; j++)
		{
			printf("%c ", matriz[i][j]);
		}
		printf("\n");
	}
	printf("\nFin Matriz\n\n");
}

//Funcion que lee el archivo de entrada y guarda los elemenos en una matriz
//Salida: Matriz con los elementos leidos
char** readFile(FILE* entrada)
{
	char** matriz = createMatrix(3,3);
	fscanf(entrada,"%c %c %c\n",&matriz[0][0],&matriz[0][1],&matriz[0][2]);
	fscanf(entrada,"%c %c %c\n",&matriz[1][0],&matriz[1][1],&matriz[1][2]);
	fscanf(entrada,"%c %c %c\n",&matriz[2][0],&matriz[2][1],&matriz[2][2]);
	return matriz;
}

//Funcion que compara dos matrices (puzzle)
//Salida: Entero que indica si son iguales o no (0: distindos, 1: iguales)
int compareMatrix(char** matriz1, char** matriz2)
{
	int i,j;
	int ans = 1;
	for(i=0;i<3;i++)
	{
		for(j=0;j<3;j++)
		{
			if(matriz1[i][j] != matriz2[i][j])
			{
				ans=0;
			}
		}
	}
	return ans;
}

//Funcion que crea la matriz solucion para comparar con los estados generados
//Salida: Matriz solucion
char** createFinalAnswer()
{
	int i,j;
	char count=1+'0';
	char** finalAnswer = createMatrix(3,3);

	for(i=0;i<3;i++)
	{
		for(j=0;j<3;j++)
		{
			if(count=='9')
			{
				finalAnswer[i][j]='x';
			}
			else
			{
				finalAnswer[i][j]=count;
			}
			count++;
		}
	}
	return finalAnswer;
}

//Funcion que busca un elemento en una matriz
//Salida: Posicion del elemento en la matriz
Position searchElementMatrix(char** puzzle,char c)
{
	Position pos;
	pos.x=-1;
	pos.y=-1;
	int i,j;
	for(i=0;i<3;i++)
	{
		for(j=0;j<3;j++)
		{
			if(puzzle[i][j]==c)
			{
				pos.x=i;
				pos.y=j;
				return pos;
			}
		}
	}
	return pos;
}

//Funcion que copia una matriz
//Salida: Copia de la matriz
char** copyMatrix(char** src)
{
	char** copy = createMatrix(3,3);
	int i,j;
	for(i=0;i<3;i++)
	{
		for(j=0;j<3;j++)
		{
			copy[i][j]=src[i][j];
		}
	}
	return copy;
}

//Funcion que retorna una matriz con los elementos cambiados de otra matriz
//Salida: Matriz con dupla de elementos intercambiados
char** swap(char** puzzle,Position pos1, Position pos2)
{
	char** new = copyMatrix(puzzle);
	new[pos1.x][pos1.y]=puzzle[pos2.x][pos2.y];
	new[pos2.x][pos2.y]=puzzle[pos1.x][pos1.y];
	return new;
}

//Funcion que muestra por pantalla un arreglo de tamaño dinamico
void printArray(Array* a)
{
	int i,size;
	size = a->size;
	for(i=0;i<size;i++)
	{
		printf("%d ",a->array[i]);
	}
	printf("\n");
}

//Funcion que comprueba si un estado ha sido visitado o no
//Salida: Entero que indica si fue visitado o no
int isVisited(char** puzzle)
{
	Node* n = visited;
	while(n!=NULL)
	{
		if(compareMatrix(puzzle,n->puzzle)==1)
		{
			return 1;
		}
		n=n->next;
	}
	return 0;
}

//Funcion que retorna el tamaño de la cola
//Salida: Tamaño de la cola
int sizeQueue()
{
	Node* n = start;
	int size=0;
	while(n != NULL)
	{
		size++;
		n=n->next;
	}
	return size;
}

//Funcion que muestra por pantalla las matrices que ya han sido visitadas
void printVisited()
{
	Node* n = visited;
	while(n != NULL)
	{
		printMatrix(n->puzzle,3,3);
		n=n->next;
	}
}

//Funcion que retorna el tamaño de la lista de visitados
//Salida: Tamaño de la lista de salidas
int sizeVisited()
{
	Node* n = visited;
	int size = 0;
	while(n != NULL)
	{
		size++;
		n=n->next;
	}
	return size;
}

//Funcion que busca todos los caminos posibles que dado un puzzle inicial, se llegue al puzzle solucion
//Salida: Arreglo con las profundidades de todas las soluciones (pasos necesarios)
Array* solution(char** puzzle)
{	
	Position startPosition,swapPos; 
	enqueue(puzzle,0);
	Node* node;
	int i;
	char** finalAnswer=createFinalAnswer();
	Array* depths =(Array*)malloc(sizeof(Array));
	depths->array = NULL;
	depths->size=0;
	int size = 1;
	//int cont=0;
	addVisited(puzzle);
	char** puzzleAux=puzzle;
	if(compareMatrix(puzzle,finalAnswer)==1)
	{
		depths->array=realloc(depths->array,sizeof(int)*size);
		depths->array[size-1]=0;
		depths->size=depths->size+1;
		return depths;
	}


	while(start != NULL)
	{
		node=dequeue();
		startPosition = searchElementMatrix(node->puzzle,'x');


		for(i=0;i<4;i++)
		{

			if(startPosition.x + dx[i] < 3 && startPosition.x + dx[i] >= 0 && startPosition.y + dy[i] < 3 && startPosition.y + dy[i] >= 0)
			{
				swapPos.x=startPosition.x+dx[i];
				swapPos.y=startPosition.y+dy[i];
				puzzleAux = swap(node->puzzle,startPosition,swapPos);
				if(isVisited(puzzleAux)==0)
				{
					if(compareMatrix(puzzleAux,finalAnswer)==0)
					{
						addVisited(puzzleAux);
						enqueue(puzzleAux,(node->depth)+1);

					}		
					else
					{ 
						depths->array=realloc(depths->array,sizeof(int)*size);
						depths->array[size-1]=(node->depth)+1;
						depths->size=depths->size+1;
						size++;
					}

				}
				else
				{
					freeMatrix(puzzleAux);
				}
			}
		}
		//cont++;

		freeNode(node);		
	}
	return depths;
}

//Funcion que retorna el elemento mas pequeño de un arreglo
//Salida: Elemento mas pequeño
int min(Array* a)
{
	int size = a->size;
	int i,aux;
	int min=a->array[0];
	for(i=0;i<size-1;i++)
	{
		aux=a->array[i+1];
		if(aux<min)
		{
			min=aux;
		}
	}
	return min;
}

void inicio()
{
	FILE* entrada = fopen("Entrada.in","r");
	char** puzzle = readFile(entrada);
	fclose(entrada);
	printf("Archivo leido\n");
	printf("Puzzle de entrada: \n");
	printMatrix(puzzle,3,3);
	printf("Buscando solucion...\n");
	printf("...\n");
	printf("...\n");
	printf("...\n");
	Array* depths = solution(puzzle);
	int movimientos = min(depths);
	printf("Se requieren %d movimientos\n", movimientos);
	FILE* salida = fopen("Salida.out","w");
	fprintf(salida,"Se requieren %d movimientos\n",movimientos);
	fclose(salida);
	printf("Archivo de salida creado con exito\n");
}