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
	// infected = 1 estao infectados
	char nome[5];
	//numero de casas ligadas
	int Nligacoes,Npessoas,infected;
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
	casas *aux;
	casaligada *aux2;

	aux = x;
	while(strcmp(aux -> nome,f)){
		aux = aux -> down;
		if(aux == NULL) return;
	}
	aux -> Nligacoes = aux -> Nligacoes + 1;
	aux2 = aux -> cl;
	if(aux2 == NULL){
		aux -> cl = criavizinho(l);
	}
	else{
		while(aux2 -> prox != NULL) aux2 = aux2 -> prox;
		aux2 -> prox = criavizinho(l);
	}
	aux = x;
	while(strcmp(aux -> nome,l)){
		aux = aux -> down;
		if(aux == NULL) return;
	}
	aux -> Nligacoes = aux -> Nligacoes + 1;
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
	while(strcmp(x -> nome,c)){
		x = x -> down;
		if(x == NULL) return;
	}
	mosquito *aedes,*aux = x -> m;
	aedes=(mosquito*)malloc(sizeof(mosquito));
	if(aedes == NULL) return;
	aedes -> Npassos = 0;
	aedes -> moved = 0;
	aedes -> prox = NULL;
	x -> infected = 1;
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
//conta numero de pessoas infectadas
int containfectado(casas *x){
	if(x == NULL) return 0;
	return (x -> Npessoas * x -> infected) + containfectado(x -> down);
}
//MOSQUITO MOVEEEE
//reseta todas as boleanas dos mosquitos para 0
//para indicar que nenhum se moveu
void movedtozero(casas *x){
	if(x == NULL) return;
	mosquito *aux = x-> m;
	while(aux != NULL){
		aux -> moved = 0;
		aux = aux -> prox;
	}
	movedtozero(x -> down);
}

void mosquito_move(casas*x){
	movedtozero(x);
	casas *aux = x,*aux2;
	mosquito **auxm,*auxm2,*auxm3;
	casaligada *auxcl;
	int n;

	while(aux != NULL){
		auxm = &aux -> m;
		while(*auxm != NULL){
			if((*auxm) -> moved == 1){
				auxm = &(*auxm) -> prox;
				continue;
			}
			aux2 = x;
			auxcl = aux -> cl;
			//seleciona a casa randomicamente
			n = rand()%(aux->Nligacoes);
			while(n){
				auxcl = auxcl -> prox;
				n--;
			}
			//acha a casa selecionada
			while(strcmp(aux2 -> nome,auxcl -> nome)){
				aux2 = aux2 -> down;
				if(aux2 == NULL) return;
			}
			aux2 -> infected = 1;
			auxm2 = (*auxm) -> prox;
			(*auxm) -> moved = 1;
			if(aux2 -> m == NULL){
				aux2 -> m = *auxm;
				(*auxm) -> prox = NULL;
				*auxm = auxm2;
				continue;
			}
			auxm3 = aux2 -> m;
			while(auxm3 -> prox != NULL) auxm3 = auxm3 -> prox;
			auxm3->prox = *auxm;
			(*auxm) -> prox = NULL;
			*auxm = auxm2;
		}
		aux = aux -> down;
	}

}
//mosquito bota
void mosquito_bota(casas *x,int n){
	casas *aux = x;
	mosquito *auxm;
	while(aux != NULL){
		auxm = aux -> m;
		while(auxm != NULL){
			if(auxm ->Npassos == n){
				auxm -> Npassos = 0;
				inseremosquito(aux,aux -> nome);
				inseremosquito(aux,aux -> nome);
			}
		}
	}
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

int main(){
	srand(time(NULL));
	char v[5],f[5];
	int n;
	casas *teste = NULL;
	for(int i = 0; i < 4; i++){
		printf("%i: ",i);
		scanf("%s %i",v,&n);
		inserecasa(&teste,v,n);
	}
	for(int i = 0; i < 3; i++){
		printf("%i: ",i);
		scanf("%s%s",v,f);
		ligacasas(teste,v,f);
	}
	for(int i = 0; i < 7; i++){
		printf("%i: ",i);
		scanf("%s",v);
		inseremosquito(teste,v);
	}
	puts("");
	imprime(teste);
	mosquito_move(teste);
	imprime(teste);
	mosquito_move(teste);
	imprime(teste);
	mosquito_move(teste);
	imprime(teste);
}
