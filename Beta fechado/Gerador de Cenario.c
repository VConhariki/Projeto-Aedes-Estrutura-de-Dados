#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(){
	int i,n = 1;
	char nome[5];
	FILE *ent;
	ent = fopen("entrada.txt","w");
	if(ent == NULL) return 0;

	printf("Digite o valor do AGENTE_ATUA:");
	scanf("%i",&n);
	fprintf(ent,"AGENTE_ATUA %i\n",n);

	printf("Digite o valor do AGENTE_SAUDE:");
	scanf("%i",&n);
	fprintf(ent,"AGENTE_SAUDE %i\n",n);

	printf("Digite o valor do MOSQUITO_BOTA:");
	scanf("%i",&n);
	fprintf(ent,"MOSQUITO_BOTA %i\n",n);
	printf("Inserçao de casas:\n");
	while (n) {
		printf("Nome da casa e numero de moradores(-1 -1 STOP):");
		scanf("%s%i",nome,&i);
		if(atoi(nome) < 0) break;
		fprintf(ent,"inserecasa %s %i\n",nome,i);
	}
	printf("Inserçao de vizinhos:\n");
	n = 1;
	strcpy(nome,"xx");
	while (n) {
		printf("Nome da casa 1 e Nome da casa 2(-1 STOP):");
		scanf("%s",nome);
		if(atoi(nome) < 0) break;
		fprintf(ent,"ligacasas %s ",nome);
		scanf("%s",nome);
		fprintf(ent,"%s\n",nome);
	}
	printf("Inserçao de mosquitos:\n");
	n = 1;
	while (n) {
		printf("Digite a casa(-1 STOP):");
		scanf("%s",nome);
		if(atoi(nome) < 0) break;
		fprintf(ent,"inseremosquito %s\n",nome);
	}

	printf("Parametro do iniciasimulacao:");
	scanf("%i",&n);
	fprintf(ent,"iniciasimulacao %i\n",n);

	fprintf(ent,"verificasurto\n");
	fprintf(ent,"FIM\n");
	fclose(ent);
}
