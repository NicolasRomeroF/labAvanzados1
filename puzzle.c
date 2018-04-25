#include <stdio.h>
#include <stdlib.h>


int dx[4] = {0,0,-1,1};
int dy[4] = {-1,1,0,0};

typedef struct Node
{
	struct Node next*;
	char** puzzle;
	int depth;
}Node;

typedef struct Position
{
	int x,y;
}Position;

struct Node* start = NULL;
struct Node* end = NULL;

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

void dequeue()
{
	Node* node = front;
	if(start == NULL)
	{
		return;
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
	free(node);
}

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

void printMatrix(char** matriz, int N, int M)
/*
	Funcion que se encarga de mostrar la sopa de letras por pantalla
	Recibe la matriz (sopa de letras) y sus dimensiones
*/
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

char** readFile(FILE* entrada)
{
	char** matriz = createMatrix(3,3);
	fscanf(entrada,"%c %c %c\n",&matriz[0][0],&matriz[0][1],&matriz[0][2]);
	fscanf(entrada,"%c %c %c\n",&matriz[1][0],&matriz[1][1],&matriz[1][2]);
	fscanf(entrada,"%c %c %c\n",&matriz[2][0],&matriz[2][1],&matriz[2][2]);
	return matriz;
}

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

Position searchElementMatrix(char** puzzle,char c)
{
	Position pos;
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
}

int solution(char** puzzle)
{
	Position startPosition = searchElementMatrix(puzzle,'x');
}

int main()
{
	FILE* entrada = fopen("entrada.in","r");
	char** puzzle = readFile(entrada);
	printMatrix(puzzle,3,3);
	char** finalAnswer=createFinalAnswer();
	printMatrix(finalAnswer,3,3);

	printf("Ans: ");
	printf("%d\n",compareMatrix(puzzle,finalAnswer));

	return 0;

}
