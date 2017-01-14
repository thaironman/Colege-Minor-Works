#include <stdio.h>
#include <stdlib.h>
#include "timer.h"  //para usar funcoes de medida de tempo

void iniciaQtd(int *qtdChar){
	int i;
	for( i=0; i<71; i++){
		qtdChar[i] = 0;
	}
}

void conta(int *qtdChar, char *caracteres, char *texto){
	int i , j = 0;
	while( texto[j]!='\0' ){
		i = 0;
		while( texto[j] != caracteres[i] && i < 71){
			i++;
		}
		if( texto[j] == caracteres[i] )
			qtdChar[i]++;
		j++;
	}
}

//funcao principal
int main(int argc, char *argv[]){
	int i;
	FILE *arq;
	int qtdChar[71];
	char caracteres[] = "?!.:;_-()ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
	char texto[100];
	double inicio, fim, delta1, delta2, delta3; //variaveis para medir o tempo de execucao

	GET_TIME(inicio);

	if (argc < 2) {
		fprintf(stderr, "Digite: %s <Arquivo txt>.\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	
	arq=fopen( argv[1], "r");
	if (arq == NULL) {
		fprintf(stderr, "Erro ao abrir o arquivo de texto.\n");
		exit(EXIT_FAILURE);
	}
	
	iniciaQtd(qtdChar);
	GET_TIME(fim);

  	//calcula o tempo gasto com as inicializacoes
  	delta1 = fim - inicio;
	GET_TIME(inicio);

	while(fgets(texto,100,arq)!=NULL){
		conta(qtdChar, caracteres, texto);
	}

	//calcula o tempo gasto com a parte concorrente
  	GET_TIME(fim)
  	delta2 = fim - inicio;
	GET_TIME(inicio);

	fclose(arq);
	
	for(i=0;i<71;i++){
		if(qtdChar[i]!=0)
			printf("%c, %d\n", caracteres[i], qtdChar[i]);
	}
	GET_TIME(fim);

	//calcula o tempo gasto com as finalizacoes 
	delta3 = fim - inicio;
	//exibe os tempos gastos em cada parte do programa 
	printf("Tempo inicializacoes: %.8lf\n", delta1);
	printf("Tempo sequencial: %.8lf\n", delta2);
	printf("Tempo finalizacoes: %.8lf\n", delta3);

	pthread_exit(NULL);
	return 0;
}