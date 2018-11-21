/*  17/11/2018
 * Algoritmo de Prim
 * Encontrar Arvore Geradora Minima (AGM)
 */

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

// Estrutura da Lista de Adjacencia
struct listaAdj{
    int destino;
    float peso;
    struct listaAdj* prox;
};


struct listaAdjac{
    struct listaAdj* cabecalho; 
};

// Estrutura do Grafo
struct Grafo {
    int V;
    struct listaAdjac* vet;
};

// Criar nova lista de adjacencia
struct listaAdj* novaLista(int destino, int peso)
{
    struct listaAdj* nova=(struct listaAdj*)malloc(sizeof(struct listaAdj));
    nova->destino = destino;
    nova->peso = peso;
    nova->prox = NULL;
    return nova;
}

// Criar Grafo
struct Grafo* criaGrafo(int V)
{
    struct Grafo* gr=(struct Grafo*)malloc(sizeof(struct Grafo));
    gr->V = V;

    
    gr->vet=(struct listaAdjac*)malloc(V * sizeof(struct listaAdjac));

    
    for (int i = 0; i < V; ++i)
        gr->vet[i].cabecalho=NULL;

    return gr;
}

// Inserir Aresta
void insereAresta(struct Grafo* gr, int origem, int destino, float peso)
{

    
    struct listaAdj *nova=novaLista(destino, peso);
    nova->prox=gr->vet[origem].cabecalho;
    gr->vet[origem].cabecalho=nova;

    
    nova=novaLista(origem, peso);
    nova->prox=gr->vet[destino].cabecalho;
    gr->vet[destino].cabecalho=nova;
}

/*
 Uso de Heap para pegar as arestas em ordem decrescente
*/

// Estrtura do Heap
struct HEAPmin{
    int v;
    int ch;
};

// Estrutura do Heap
struct heapMin{
    int tamanho; 
    int n; 
    int* pos; 
    struct HEAPmin** vet;
};

// Criar novo Heap
struct HEAPmin* novaHeap(int v, int ch)
{
    struct HEAPmin* heap=(struct HEAPmin*)malloc(sizeof(struct HEAPmin));
    heap->v=v;
    heap->ch=ch;
    return heap;
}

// Criar o Heap
struct heapMin* criaHeap(int n)
{
    struct heapMin*hm=(struct heapMin*)malloc(sizeof(struct heapMin));
    hm->pos=(int*)malloc(n* sizeof(int));
    hm->tamanho=0;
    hm->n=n;
    hm->vet = (struct HEAPmin**)malloc(n*sizeof(struct HEAPmin*));
    return hm;
}

// Troca de Valores de acordo com a propriedade Heap Minimo
void troca(struct HEAPmin**n1, struct HEAPmin**n2)
{
    struct HEAPmin*temp= *n1;
    *n1=*n2;
    *n2=temp;
}

// Funçao HeapFy para satisfazer as propriedades do Heap 
void heapfy(struct heapMin *hm, int id)
{
    int menor, esq, dir;
    menor = id;
    esq = 2 * id + 1;
    dir = 2 * id + 2;

    if (esq < hm->tamanho && hm->vet[esq]->ch<hm->vet[menor]->ch)
        menor=esq;

    if (dir < hm->tamanho && hm->vet[dir]->ch<hm->vet[menor]->ch)
        menor = dir;

    if (menor != id) {
      
        HEAPmin* min = hm->vet[menor];
        HEAPmin* ident = hm->vet[id];

        
        hm->pos[min->v] = id;
        hm->pos[ident->v] = menor;

        
        troca(&hm->vet[menor], &hm->vet[id]);

        heapfy(hm, menor);
    }
}

// Verificar se o heap esta vazio
int heapVazia(struct heapMin* hm)
{
    return hm->tamanho==0;
}

// Extrair o minimo do Heap
struct HEAPmin* extractMin(struct heapMin* hm)
{
    if (heapVazia(hm))
        return NULL;

   
    struct HEAPmin* raiz = hm->vet[0];

    
    struct HEAPmin* ultimo = hm->vet[hm->tamanho-1];
    hm->vet[0]=ultimo;

    
    hm->pos[raiz->v] = hm->tamanho-1;
    hm->pos[ultimo->v] = 0;

    
    --hm->tamanho;
    heapfy(hm, 0);

    return raiz;
}

// Reduzir o Heap
void reduz(struct heapMin* hm, int v, int ch)
{
    
    int i = hm->pos[v];

    
    hm->vet[i]->ch=ch;

  
    while(i && hm->vet[i]->ch < hm->vet[(i - 1) / 2]->ch) {
       
        hm->pos[hm->vet[i]->v] = (i - 1) / 2;
        hm->pos[hm->vet[(i - 1) / 2]->v] = i;
        troca(&hm->vet[i], &hm->vet[(i - 1) / 2]);

        i = (i - 1) / 2;
	}
}

// Verificar se o valor esta no Heap
bool valor(struct heapMin* hm, int v)
{
    if (hm->pos[v] < hm->tamanho)
        return true;
    return false;
}

// Impressao na Tela
void imprime(int a[], int n)
{
    for (int i = 1; i < n; ++i)
        printf("%d - %d\n", a[i], i);
}

// Algoritmo de Prim
void Prim(struct Grafo* gr)
{
    int V = gr->V; 
    int pai[V]; 
    int ch[V]; 

    
    struct heapMin* hm=criaHeap(V);

  
    for (int v = 1; v < V; ++v) {
        pai[v] = -1;
        ch[v] = INT_MAX;
        hm->vet[v] = novaHeap(v, ch[v]);
        hm->pos[v] = v;
    }

    
    ch[0] = 0;
    hm->vet[0]=novaHeap(0, ch[0]);
    hm->pos[0]=0;

    hm->tamanho=V;

    
    while (!heapVazia(hm)) {
        
        struct HEAPmin* heap = extractMin(hm);
        int u = heap->v; 

        struct listaAdj* p = gr->vet[u].cabecalho;
        while (p!=NULL) {
            int v = p->destino;

           
            if (valor(hm, v) && p->peso < ch[v]) {
                ch[v] = p->peso;
                pai[v] = u;
                reduz(hm, v, ch[v]);
            }
            p= p->prox;
        }
    }

   
    imprime(pai, V);
}

//Preenchimento do Grafo com o arquivo de entrada para executar o k-agrupamento 
void preencheGrafo(Grafo *gr){
    FILE *arq;

	arq = fopen("dados.txt", "r");
	if(arq == NULL)
			printf("ERRO NA ABERTURA DO ARQUIVO\n");

    int i = 0, origem, destino;
    float peso;

		while((fscanf(arq,"%d %d %f\n", &origem, &destino, &peso))!=EOF){
            insereAresta(gr, origem, destino, peso);
		}

}

// Funcao Principal
int main()
{
	int k;
	printf("\n\nDigite o valor de k para o k-agrupamento:\n");
    scanf("%d", &k);

    struct Grafo* gr=criaGrafo(k); 

    preencheGrafo(gr);

    Prim(gr);

    return 0;
}
