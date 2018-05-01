#include <stdio.h>
#include <stdlib.h>


int dx[4] = {0,0,-1,1};
int dy[4] = {-1,1,0,0};

typedef struct Node
{
	struct Node* next;
	char** puzzle;
	int depth;
}Node;

typedef struct Position
{
	int x,y;
}Position;

typedef struct Array
{
	int* array;
	int size;
}Array;

struct Node* start = NULL;
struct Node* end = NULL;
struct Node* visited =NULL;
struct Node* visitedEnd =NULL;

void enqueue(char** puzzle, int depth)
{
	//printf("enqueue---------------\n ");
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

void addVisited(char** puzzle)
{
	//printf("enqueue---------------\n ");
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

Node* dequeue()
{
	//printf("dequeue---------------\n ");
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

void freeMatrix(char** matrix)
{
	int i;
	for(i=0;i<3;i++)
	{
		free(matrix[i]);
	}
	free(matrix);
}

void freeNode(Node* n)
{
	//freeMatrix(n->puzzle);
	free(n);
}

char** createMatrix(int N, int M)
{
	//printf("createMatrix---------------\n ");
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

void printMatrix(char** matriz, int N, int M)
{
	printf("printMatrix---------------\n ");
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

char** readFile(FILE* entrada)
{
	printf("readFile---------------\n ");
	char** matriz = createMatrix(3,3);
	fscanf(entrada,"%c %c %c\n",&matriz[0][0],&matriz[0][1],&matriz[0][2]);
	fscanf(entrada,"%c %c %c\n",&matriz[1][0],&matriz[1][1],&matriz[1][2]);
	fscanf(entrada,"%c %c %c\n",&matriz[2][0],&matriz[2][1],&matriz[2][2]);
	return matriz;
}

int compareMatrix(char** matriz1, char** matriz2)
{
	//printf("compareMatrix---------------\n ");
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

char** createFinalAnswer()
{
	//printf("createFinalAnswer---------------\n ");
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

Position searchElementMatrix(char** puzzle,char c)
{
	//printf("searchElementMatrix---------------\n ");
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

char** copyMatrix(char** src)
{
	//printf("copy---------------\n ");
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

char** swap(char** puzzle,Position pos1, Position pos2)
{
	//printf("swap---------------\n ");
	char** new = copyMatrix(puzzle);
	new[pos1.x][pos1.y]=puzzle[pos2.x][pos2.y];
	new[pos2.x][pos2.y]=puzzle[pos1.x][pos1.y];
	return new;
}

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

int isVisited(char** puzzle)
{
	//printf("isVisited---------------\n ");
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

void printVisited()
{
	//printf("VISITED-----------------------------------");
	Node* n = visited;
	while(n != NULL)
	{
		printMatrix(n->puzzle,3,3);
		n=n->next;
	}
}
int sizeVisited()
{
	//printf("VISITED-----------------------------------");
	Node* n = visited;
	int size = 0;
	while(n != NULL)
	{
		size++;
		n=n->next;
	}
	return size;
}

int compareVisited()
{
	Node* n = visited;
	Node* next = n->next;
	int cont = 0;
	while(n != NULL)
	{
		while(next != NULL)
		{
			if(compareMatrix(n->puzzle,next->puzzle)==1)
			{
				return 1;
			}
			next = next->next;
			cont++;
		}
		n=n->next;
	}
	printf("cont: %d\n",cont);
	return 0;
}

Array* solution(char** puzzle)
{
	//printf("solution---------------\n ");
	
	Position startPosition,swapPos; 
	enqueue(puzzle,0);
	Node* node;
	int i;
	char** finalAnswer=createFinalAnswer();
	Array* depths =(Array*)malloc(sizeof(Array));
	depths->array = NULL;
	depths->size=0;
	int size = 1;
	//printf("ok1");
	int cont=0;
	addVisited(puzzle);
	char** puzzleAux=puzzle;
	while(start != NULL)
	{
		//printf("Queue size bef: %d\n",sizeQueue());

		node=dequeue();
		startPosition = searchElementMatrix(node->puzzle,'x');
		printf("cont: %d for: %d\n",cont,i);
		printf("Queue size: %d\n",sizeQueue());
		printf("Visited size: %d\n",sizeVisited());

		for(i=0;i<4;i++)
		{
			//printf("cont: %d for: %d\n",cont,i);
			//printMatrix(node->puzzle,3,3);

			
			if(startPosition.x + dx[i] < 3 && startPosition.x + dx[i] >= 0 && startPosition.y + dy[i] < 3 && startPosition.y + dy[i] >= 0)
			{
				swapPos.x=startPosition.x+dx[i];
				swapPos.y=startPosition.y+dy[i];
				puzzleAux = swap(node->puzzle,startPosition,swapPos);
				//printf("start x: %d y: %d\n",startPosition.x,startPosition.y);
				if(isVisited(puzzleAux)==0)
				{
					addVisited(puzzleAux);
					enqueue(puzzleAux,(node->depth)+1);

					//printf("cont: %d for: %d\n",cont,i);
					//printVisited();

					//printf("COMPARE-----------------\n");
					//printMatrix(node->puzzle,3,3);
					//printMatrix(puzzleAux,3,3);
					//printf("if\n");
					
					if(compareMatrix(puzzleAux,finalAnswer))
					{ 
						//printf("IF COMPARE\n");
						depths->array=realloc(depths->array,sizeof(int)*size);
						depths->array[size-1]=(node->depth)+1;
						depths->size=depths->size+1;
						size++;
						
					}

				}
				else
				{
					//printf("VISITED\n");
					freeMatrix(puzzleAux);
				}
			}
		}
		cont++;
		//printf("antes free\n");
		freeNode(node);
		//printf("Queue size: %d\n",sizeQueue());
		
	}
	printf("Queue size: %d\n",sizeQueue());
	printf("cont: %d\n",cont);

	printf("OVER-- size: %d\n",size);
	printf("compareVisited: %d\n",compareVisited());
	printArray(depths);
	printf("size array: %d\n",depths->size);
	return depths;
}

int max(Array* a)
{
	//printf("max---------------\n ");
	int size = a->size;
	//printf("sizeArray: %d\n",size);
	int i,aux;
	int max=a->array[0];
	for(i=0;i<size-1;i++)
	{
		aux=a->array[i+1];
		if(aux>max)
		{
			max=aux;
		}
	}
	return max;
}


int main()
{
	//printf("wat?\n");
	FILE* entrada = fopen("Entrada.in","r");
	char** puzzle = readFile(entrada);
	fclose(entrada);
	printf("Archivo leido\n");
	printMatrix(puzzle,3,3);
	Array* depths = solution(puzzle);

	printf("Se requieren %d movimientos\n",max(depths));
	FILE* salida = fopen("Salida.out","w");
	fprintf(salida,"Se requieren %d movimientos\n",max(depths));
	fclose(salida);

	return 0;
}
