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

//struct Node* start = NULL;
//struct Node* end = NULL;

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
	//printf("printMatrix---------------\n ");
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

void freeMatrix(char** matrix)
{
	int i;
	for(i=0;i<3;i++)
	{
		free(matrix[i]);
	}
	free(matrix);
}

void printArray(Array a)
{
	int i,size;
	size = a.size;
	for(i=0;i<size;i++)
	{
		printf("%d ",a.array[i]);
	}
	printf("\n");
}

Array solution(char** puzzle)
{
	printf("solution---------------\n ");
	Position startPosition,swapPos; 
	enqueue(puzzle,0);
	int flag = 1;
	Node* node;
	int i;
	char** finalAnswer=createFinalAnswer();
	Array depths;
	depths.array = NULL;
	depths.size=0;
	int size = 1;
	while(flag==1)
	{
		startPosition = searchElementMatrix(puzzle,'x');
		node=dequeue();
		printf("while\n");
		for(i=0;i<4;i++)
		{
			printf("for\n");
			if(startPosition.x + dx[i] < 3 && startPosition.x + dx[i] >= 0 && startPosition.y + dy[i] < 3 && startPosition.y + dy[i] >= 0)
			{
				swapPos.x=startPosition.x+dx[i];
				swapPos.y=startPosition.y+dy[i];
				char** puzzleAux = swap(node->puzzle,startPosition,swapPos);
				enqueue(puzzleAux,(node->depth)+1);
				printf("if\n");
				if(compareMatrix(puzzleAux,finalAnswer))
				{
					depths.array=realloc(depths.array,sizeof(int)*size);
					depths.array[0]=(node->depth)+1;
					depths.size=depths.size+1;
					size++;
					printArray(depths);
				}
				else
				{
					freeMatrix(puzzleAux);
				}

			}
		}
	}
	return depths;
}

int max(Array a)
{
	printf("max---------------\n ");
	int size = a.size;
	int i,aux;
	int max=a.array[0];
	for(i=0;i<size-1;i++)
	{
		aux=a.array[i+1];
		if(aux>max)
		{
			max=aux;
		}
	}
	return max;
}

int main()
{
	printf("wat?\n");
	FILE* entrada = fopen("entrada.in","r");
	char** puzzle = readFile(entrada);
	printf("Archivo leido\n");
	printMatrix(puzzle,3,3);
	

	printf("Ans: ");
	Array depths = solution(puzzle);
	printf("%d",max(depths));

	return 0;
}
