#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//estrutura para casas ligadas
typedef struct casaligada{
	char nome[5];
	struct casaligada *prox;
}casaligada;

//estrutura mosquito
typedef struct mosquito{
	//conta o numero de passo
	int Npassos;
	//bool para verificar se ja se mecheu quando a funçao
	//mosquitomove() for chamada
	char moved;
	struct mosquito *prox;
}mosquito;
/*
exemplo de casa principal
###############
# nome = casa3#
# infected = 1#
# nlig = 2    #
###############         ###########        #############
# ponteiro cl-#-------> # casa 1--#------> # casa 2  --#----|I-
###############         ###########        #############
# ponteiro m -#-------> # msqt 1--#----|I-
###############         ###########

*/
//ESTRUTURA PRINCIPAL
typedef struct casaprincipal{
	//infected é uma boleana para saber se a casa esta infectada ou nao
	char nome[5];
	int infected;
	//numero de casas ligadas
	int Nligacoes,Npessoas;
	//estruturas conexoes e mosquitos
	casaligada *cl;
	mosquito *m;
	struct casaprincipal *down;
}casas;


///mudar tudo abaixo devido a alteraços nas structs
//insersao das casas na coluna principal
void inserecasa(casas **x,char v[],int np){
	casas *aux = (casas*)malloc(sizeof(casas));
	if(aux == NULL) return;
	strcpy(aux -> nome,v);
	aux -> infected = 0;
	aux -> Nligacoes = 0;
	aux -> Npessoas = np;
	aux -> m = NULL;
	aux -> cl = NULL;
	aux -> down = NULL;
	if(*x == NULL){
		*x = aux;
		return;
	}
	while((*x) -> down != NULL) x = &(*x) -> down;
	(*x) -> down = aux;
}

//insersao das casas vizinhas
casaligada * criavizinho(char nome[]){
	casaligada *aux = (casaligada*)malloc(sizeof(casaligada));
	if(aux == NULL) return NULL;
	strcpy(aux -> nome,nome);
	aux -> prox = NULL;
	return aux;

}

void ligacasas(casas *x,char f[],char l[]){
	casas *aux = x;
	casaligada *aux2;

	while(strcmp(aux -> nome,f)) aux = aux -> down;
	aux2 = aux -> cl;
	if(aux2 == NULL){
		aux -> cl = criavizinho(l);
	}
	else{
		while(aux2 -> prox != NULL) aux2 = aux2 -> prox;
		aux2 -> prox = criavizinho(l);
	}
	aux = x;
	while(strcmp(aux -> nome,l)) aux = aux -> down;
	aux2 = aux -> cl;
	if(aux2 == NULL){
		aux -> cl = criavizinho(f);
	}
	else{
		while(aux2 -> prox != NULL) aux2 = aux2 -> prox;
		aux2 -> prox = criavizinho(f);
	}
}

//insersao de mosquitos
void inseremosquito(casas *x,char c[]){
	if(x == NULL) return;
	while(strcmp(x -> nome,c)) x = x -> down;
	mosquito *aedes,*aux = x -> m;
	aedes=(mosquito*)malloc(sizeof(mosquito));
	if(aedes == NULL) return;
	aedes -> Npassos = 0;
	aedes -> moved = 0;
	aedes -> prox = NULL;
	x->infected = 1;
	if(aux == NULL){
		x -> m = aedes;
		return;
	}
	while(aux -> prox != NULL) aux = aux -> prox;
	aux -> prox = aedes;
}

//conta numeros de mosquitos
int contamosquito(casas *x){
	if(x == NULL) return 0;
	int n = 0;
	mosquito *aux = x -> m;
	while(aux != NULL){
		n++;
		aux = aux -> prox;
	}
	return n + contamosquito(x -> down);
}
//impressoes, TRANSFORMAR NA FUNÇAO DE GERAÇAO DE LOG!!
void imprimeligacoes(casaligada *x){
	if(x == NULL){
		return;
	}
	printf("%s -> ",x -> nome);
	imprimeligacoes(x -> prox);
}

void imprimemosquitos(mosquito *x){
	if(x == NULL){
		return;
	}
	printf("M -> ");
	imprimemosquitos(x -> prox);
}

void imprime(casas *x){
	if(x == NULL){
		printf("\n");
		return;
	}
	printf("%s -> ",x -> nome);
	imprimeligacoes(x -> cl);
	imprimemosquitos(x -> m);
	printf("\n");
	imprime(x -> down);
}
//impressao
void desaloca(mosquito **p){

	if(*p == NULL) return ;
	
	desaloca(&(*p)->prox);
	free(*p);
	*p = NULL;
	
}

void imprimeinfected(casas **p){

	if(*p == NULL){
		puts("");
		return ;
	}
	
	printf("%d ",(*p)->infected);
	imprimeinfected(&(*p)->down);

}

int agente_atua(casas *p, int cont){
	
	casas *aux = p;
	int n=0;
	
	n=rand()%cont+1;
	printf("%d\n",n);
	for(int i=1;i<n;i++) aux = aux->down;
	
	if(aux->m == NULL) return 0;
	desaloca(&aux->m);
	return 1;
		
}

int agente_saude(casas *p, int cont){

	casas *aux = p;
	int n=0;
	
	n=rand()%cont+1;
	printf("%d\n",n);
	for(int i=1;i<n;i++) aux = aux->down;
	
	if(aux->infected == 0) return 0;
	if(aux->m != NULL) desaloca(&aux->m); // mata mosquito
	aux->infected = 0;
	return 1;

}

int main(){
	srand(time(NULL));
	char v[5],f[5];
	int n,maincont=0;
	casas *teste = NULL;
	for(int i = 0; i < 2; i++){
		printf("%i: ",i);
		scanf("%s %i",v,&n);
		inserecasa(&teste,v,n);
		maincont++;
	}
	for(int i = 0; i < 2; i++){
		printf("%i: ",i);
		scanf("%s%s",v,f);
		ligacasas(teste,v,f);
	}
	for(int i = 0; i < 2; i++){
		printf("%i: ",i);
		scanf("%s",v);
		inseremosquito(teste,v);
	}
	imprime(teste);
	printf("%i\n",contamosquito(teste));
	//agente_atua(teste,maincont);
	imprimeinfected(&teste);
	imprime(teste);
	agente_saude(teste,maincont);
	imprimeinfected(&teste);
	imprime(teste);
	

}
