/* 17/11/2018
 * Algoritmo de Kruskal
 * Encontrar Arvore Geradora Minima (AGM) e fazer k-agrupamento
 */

#include <stdio.h>
#include<stdlib.h>
#include<dir.h>
#define N 4
int aux=N; //N eh o numero de vertices
int k;
typedef struct sAresta
{
    int v1;
    int v2;
    double peso;
}aresta;

 
int ciclo[N+1];	//vetor utilizado para a detecçao de ciclos tamanho é a quantidade maxima de vertices
 
void unir(int v1, int v2); //detectar ciclos
int pai(int x);				//detectar ciclos
void insertionSort(aresta *A, int n);	//ordenar as arestas pelo peso
 

int main ()
{
	
	int i;

    aresta *A; //vetor para armazenar as arestas:
	A = (aresta*)malloc((N*N) * sizeof(aresta));

	//iniciando o vetor de ciclos
	for(i=1; i<N; i++)
	{ //se os vertices forem numerados de 0 a n colocar n+1 no lugar de n
		ciclo[i] = i;
	}
	//Leitura das arestas pelo arquivo
	char dados[10];	

	sprintf(dados, "dados.txt");
	FILE *arq;
	arq=fopen(dados, "rt"); 	
	if(arq==NULL)
	{
		printf("ERRO NA ABERTURA DO ARQUIVO");
		return 0;
	}

	//lendo e armazenando as arestas
	printf("Arestas e peso:");
	i=0;
	while (!feof(arq))
	{
		fscanf(arq, "%d %d %f", &A[i].v1, &A[i].v2, &A[i].peso);
		i++;
	}

	printf("\n\nDigite o valor de k para o k-agrupamento:\n");
    scanf("%d", &k);

	//ordenando pelo peso
	insertionSort(A, N*N);

	//kruskal
	for(i=0; i<N; i++)
	{ //percorrendo todas as arestas
	
		//detectando se com esta aresta forma ciclo:
		if ( pai(A[i].v1) != pai(A[i].v2) )
		{ 
			unir(A[i].v1, A[i].v2);
		}
	}
	
	printf("1 2 3 4 ---- VERTICES\n");
	for(i=1; i<=N; i++)
	{
		printf("%d ", ciclo[i]);
	}
	printf("---- ROTULOS\n");
	fclose(arq);	
	free(A);
	
	return 0;	
}
 
 
void unir(int v1, int v2)
{
    if(k==aux)
    {
		return; 
	}
	ciclo[pai(v1)] = pai(v2);
	aux--;
}

int pai(int v)
{
 
    if (ciclo[v]==v)
	{
        return v;
	}
 
    ciclo[v] = pai(ciclo[v]);
 
    return ciclo[v];
}
 
//Insertion Sort 
void insertionSort(aresta *A, int n) 
{
    int i, j;
    float aux; 
 
    for (i = 1; i < n; i++) 
	{
        aux=A[i].peso;
		int v1=A[i].v1;
		int v2=A[i].v2; 
 		i=j-1;
 		
        for (j>0; j > -1&&A[i].peso>aux; j--)
		{
			A[i+1].peso=A[i].peso;
        	A[i+1].v1 =A[i].v1;
        	A[i+1].v2=A[i].v2;
        }
    	A[i+1].peso=aux;
        A[i+1].v1=v1;
       	A[i+1].v2=v2;
	}
}


