#include <iostream>

using namespace std;

struct PtHeap {
	int u, v, peso;
	PtHeap *left;
	PtHeap *right;
};

/*struct PtLista {
	int u, v, peso;
	PtLista *prox;
};*/

void initHeap(PtHeap *&pt, int u, int v, int peso){
	pt = new PtHeap();
	pt->u = u;
	pt->v = v;
	pt->peso = peso;
	pt->left = NULL;
	pt->right = NULL;
}

/*void lista(PtLista *&pt, int u, int v, int peso){
	if (pt != NULL){
		if (pt->prox == NULL){
			PtLista* aux = new PtLista();
			aux->u = u;
			aux->v = v;
			aux->peso = peso;
			aux->prox = NULL;
			pt->prox = aux;
		}
		else{
			lista(pt->prox, u, v, peso);
		}
	}
	else{
		pt = new PtLista();
		pt->u = u;
		pt->v = v;
		pt->peso = peso;
		pt->prox = NULL;
	}
}*/

void verifica(int** &heap, int n, int no){
	int aux = 0;
	if (n % 2 == 0){
		if (no * 2 == n || (no * 2) + 1 == n){		// se n é par e é o no da ultima arvore
			if (heap[no][2] < heap[no * 2][2]){
				aux = heap[no][2];
				heap[no][2] = heap[no * 2][2];
				heap[no * 2][2] = aux;
				aux = heap[no][1];
				heap[no][1] = heap[no * 2][1];
				heap[no * 2][1] = aux;
				aux = heap[no][0];
				heap[no][0] = heap[no * 2][0];
				heap[no * 2][0] = aux;

				if (no*4<=n)
					verifica(heap, n, no * 2);
			}
		}
	}
	else{	// se n é impar, ou é um no qualquer
		if (heap[no][2] < heap[no * 2][2] || heap[no][2] < heap[(no * 2) + 1][2]){
			if (heap[no * 2][2] <= heap[(no * 2) + 1][2]){
				aux = heap[no][2];
				heap[no][2] = heap[no * 2][2];
				heap[no * 2][2] = aux;
				aux = heap[no][1];
				heap[no][1] = heap[no * 2][1];
				heap[no * 2][1] = aux;
				aux = heap[no][0];
				heap[no][0] = heap[no * 2][0];
				heap[no * 2][0] = aux;

				if (no * 4 <= n)
					verifica(heap, n, no * 2);
			}
			else{
				aux = heap[no][2];
				heap[no][2] = heap[(no * 2) + 1][2];
				heap[(no * 2) + 1][2] = aux;
				aux = heap[no][1];
				heap[no][1] = heap[(no * 2) + 1][1];
				heap[(no * 2) + 1][1] = aux;
				aux = heap[no][0];
				heap[no][0] = heap[(no * 2) + 1][0];
				heap[(no * 2) + 1][0] = aux;

				if ((no * 4) + 2 <= n)
					verifica(heap, n, no * 2);
			}
		}
	}
}

void minHeapify(int** &heap, int n){
	int i = n/2;
	while (i > 0){
		verifica(heap, n, i);
		i -= 1;
	}

}



int main(){

	int vertices, arestas, u, v, peso, n, i, j, menorPeso, maiorPeso;
	n = 0;

	cin >> vertices >> arestas;

	int** aresta = new int*[arestas];
	for ( i = 0; i < arestas; i++)
		aresta[i] = new int[4]; // 0 = u, 1 = v, 2 = peso, 3 = estado (0 incluido, 1 não visto, -1 retirado)

	for ( i = 0; i < arestas; i++){	//recebe entradas
		cin >> u >> v >> peso;
		aresta[i][0] = u;
		aresta[i][1] = v;
		aresta[i][2] = peso;
		aresta[i][3] = 1;
	}

	int* vertice = new int[vertices+1];		// define vetor de vertices
	for (i = 0; i < vertices+1 ; i++)
		vertice[i] = 0;

	for (i = 0; i < arestas; i++){		// define maior peso
		if (i == 0)
			maiorPeso = aresta[i][2];
		if (aresta[i][2] > maiorPeso){
			maiorPeso = aresta[i][2];
		}
	}

	bool check = true;		// true se encontrou algum peso de estado 1
	while (check){
		check = false;
		menorPeso = maiorPeso;
		j = 0;
		u = 0;
		v = 0;
		for (i = 0; i < arestas; i++){		// verifica se ainda há menor peso
			if (aresta[i][2] <= menorPeso && aresta[i][3] == 1){
				menorPeso = aresta[i][2];
				j = i;
				u = aresta[i][0];
				v = aresta[i][1];
				check = true;
			}
		}
		if (check){		// verifica se fecha ciclo ou não

			n += 1;		// existirão n elementos na arvore de menor peso
			if (vertice[u] == 0){  
				if (vertice[v] == 0){
					vertice[u] = u;
					vertice[v] = u;
					aresta[j][3] = 0;
				}
				else{
					vertice[u] = vertice[v];
					aresta[j][3] = 0;
				}
			}
			else{
				if (vertice[v] == 0){
					vertice[v] = vertice[u];
					aresta[j][3] = 0;
				}
				else{
					if (vertice[u] == vertice[v]){		// fecha ciclo
						aresta[j][3] = -1;		// exclui da lista de arestas
						n -= 1;
					}
					else{		// junta dois caminhos
						aresta[j][3] = 0;
						v = vertice[v];
						u = vertice[u];
						for (i = 0; i < vertices; i++){
							if (vertice[i + 1] == v)
								vertice[i + 1] = u;
						}
					}
				}
			}
		}
	}		// end of while

	int** vetHeap = new int*[n+1];		// vetor da Heap
	for (i = 0; i < n+1; i++)
		vetHeap[i] = new int[3];

	j = 0;
	for (i = 0; i < arestas; i++){		//	vetor da Heap completo
		if (aresta[i][3] == 0){
			j += 1;
			vetHeap[j][0] = aresta[i][0];
			vetHeap[j][1] = aresta[i][1];
			vetHeap[j][2] = aresta[i][2];
		}			
	}

	minHeapify(vetHeap, n);

	int sum = 0;
	for (i = 1; i < n + 1; i++){
		sum = sum + vetHeap[i][2];
	}
	cout << sum;

}