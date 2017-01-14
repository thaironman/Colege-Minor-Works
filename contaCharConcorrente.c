#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "timer.h"  //para usar funcoes de medida de tempo
#define N 5
#define TAM 100

FILE *arq;
int qtdChar[76];
char caracteres[76]="?!.:;_-()@%&$#ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
char buffer[N][TAM];
int nthreads;
int saiu=0,count=0,verif=0;
pthread_mutex_t mutex,mutex_conta;
pthread_cond_t cond_cons,cond_prod;

//inicializa o vetor que contabiliza os caracteres do arquivo todo com zero
void iniciaQtd(){
	int i;
	for(i=0;i<71;i++){
		qtdChar[i]=0;
	}
}

//conta os caracteres de uma string e incrementa o vetor que contabiliza a quantidade total de caracteres do arquivo
void conta(char *buff){
	int i,j=0;
	while(buff[j]!='\0'){
		i=0;
		while(buff[j]!=caracteres[i]&&i<76){
			i++;
		}
		if(buff[j]==caracteres[i]){
			pthread_mutex_lock(&mutex_conta);
			qtdChar[i]++;
			pthread_mutex_unlock(&mutex_conta);
		}
		j++;
	}
}

//insere uma string no buffer
void insereBuffer(char *texto){
	static int in = 0;
	pthread_mutex_lock(&mutex);
	if (count==N){
		pthread_cond_wait(&cond_prod,&mutex);
	}
	strcpy(buffer[in],texto);
	in = (in+1)%N;
	count++;
	pthread_cond_signal(&cond_cons);
	pthread_mutex_unlock(&mutex);
}

//retira uma string do buffer
char *retiraBuffer(){
	static int out = 0;
	char temp[TAM];
	pthread_mutex_lock(&mutex);
	while(count==0){
		pthread_cond_wait(&cond_cons,&mutex);
	}
	if(verif==0){
		strcpy(temp,buffer[out]);
		out=(out+1)%N;
		count--;
		pthread_cond_signal(&cond_prod);
	}
	else{
		strcpy(temp,"");
	}
	pthread_mutex_unlock(&mutex);
	return temp;
}

//thread produtora
void * produtor(void *arg){
	char texto[100];
	while(fgets(texto,TAM,arq)!=NULL){	//enquanto o arquivo nao estiver sido lido por completo, a thread produtora continua
		insereBuffer(texto);
	}
	while(count!=0){	//arquivo ja lido totalmente, espera o buffer esvaziar tambem
	}
	verif=1;
	while(saiu!=nthreads-1){	//enquanto todas as threads consumidoras nao terminarem, emite um signal para que elas nao fiquem paradas
		count=1;
		pthread_cond_signal(&cond_cons);
	}
}

//thread consumidora
void * consumidor(void *arg){
	char buff[TAM];
	int ntid = *(int*) arg;

	while(verif==0){	//enquanto o arquivo nao estiver vazio as threads consumidoras continuam
		strcpy(buff,retiraBuffer());
		conta(buff);
	}
	saiu++; //incrementa ao terminar uma thread consumidora
}

//funcao principal
int main(int argc, char *argv[]){

	pthread_t *tid_sistema;
	int i;
	int *tid;
	double inicio, fim, delta1, delta2, delta3; //variaveis para medir o tempo de execucao

	GET_TIME(inicio);
	if (argc < 3) {
		fprintf(stderr, "Digite: %s <Arquivo txt> <No threads>.\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	arq = fopen(argv[1],"r");
	if (arq == NULL) {
		fprintf(stderr, "Erro ao abrir o arquivo de texto.\n");
		exit(EXIT_FAILURE);
	}
	
	nthreads = atoi(argv[2]);
	if (nthreads == 0) {
		fprintf(stderr, "Erro ao ler o número de threads.\n");
		exit(EXIT_FAILURE);
	}

	//zera o vetor que ira contabilizar os caracteres
	iniciaQtd();
	GET_TIME(fim);

  	//calcula o tempo gasto com as inicializacoes
  	delta1 = fim - inicio;
	GET_TIME(inicio);
	tid_sistema = (pthread_t *)malloc(sizeof(pthread_t) * nthreads);
	if (tid_sistema == NULL) {
		printf("--ERRO: malloc()\n"); exit(-1);
	}
	
	//cria a thread produtora
	tid = malloc(sizeof(int)); if (tid == NULL) { printf("--ERRO: malloc()\n"); exit(-1); }
	*tid = 0;

	if (pthread_create(&tid_sistema[0], NULL, produtor, (void*)tid)) {
		printf("--ERRO: pthread_create()\n"); exit(-1);
	}
	
	//cria as threads consumidoras
	for (i = 1; i<nthreads; i++) {
		tid = malloc(sizeof(int)); if (tid == NULL) { printf("--ERRO: malloc()\n"); exit(-1); }
		*tid = i;

		if (pthread_create(&tid_sistema[i], NULL, consumidor, (void*)tid)) {
			printf("--ERRO: pthread_create()\n"); exit(-1);
		}
	}
	
	
	//espera todas as threads terminarem
	int t;
	for(t=0; t<nthreads; t++) {
		if (pthread_join(tid_sistema[t], NULL)) {
			printf("--ERRO: pthread_join()\n"); exit(-1);
		}
	}
	//calcula o tempo gasto com a parte concorrente
  	GET_TIME(fim)
  	delta2 = fim - inicio;
	GET_TIME(inicio);
	fclose(arq);

	//imprime o resultado na tela
	for(i=0; i<71; i++){
		if(qtdChar[i]!=0)
			printf("%c, %d\n", caracteres[i],qtdChar[i]);
	}
	free(tid_sistema);
	free(tid);
	GET_TIME(fim);

	//calcula o tempo gasto com as finalizacoes 
	delta3 = fim - inicio;
	//exibe os tempos gastos em cada parte do programa 
	printf("Tempo inicializacoes: %.8lf\n", delta1);
	printf("Tempo concorrente com %d threads: %.8lf\n", nthreads, delta2);
	printf("Tempo finalizacoes: %.8lf\n", delta3);

	pthread_exit(NULL);
	return 0;
}