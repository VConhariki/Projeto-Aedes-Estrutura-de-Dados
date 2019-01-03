//Nome: Jefferson Michael de Azevedo Junior ra:2058979
//Nome: Vagner de Oliveira Conhariki       ra:2057530
//Arquivo extra para testes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
int contcs=0,contPassosM;

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
	casas *auxcs = x;
	while(strcmp(auxcs -> nome,c)){
		auxcs = auxcs -> down;
		if(auxcs == NULL) return;
	}
	mosquito *aedes,*aux = auxcs -> m;
	aedes=(mosquito*)malloc(sizeof(mosquito));
	if(aedes == NULL) return;
	aedes -> Npassos = 0;
	aedes -> moved = 0;
	aedes -> prox = NULL;
	auxcs -> infected = 1;
	if(aux == NULL){
		auxcs -> m = aedes;
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
			auxm2 = (*auxm) -> prox;
			(*auxm) -> moved = 1;
			(*auxm) -> Npassos += 1;
			//move o mosquito para casa que nao possui mosquito
			if(aux2 -> m == NULL){
				aux2 -> infected = 1; //infectando a casa onde o mosquito vai
				aux2 -> m = *auxm;
				(*auxm) -> prox = NULL;
				*auxm = auxm2;
				contPassosM++;//conta passos mosquito
				continue;
			}
			//move o mosquito para casa que ja possui mosquito
			aux2 -> infected = 1; //infectando a casa onde o mosquito vai
			auxm3 = aux2 -> m;
			while(auxm3 -> prox != NULL) auxm3 = auxm3 -> prox;
			auxm3->prox = *auxm;
			(*auxm) -> prox = NULL;
			*auxm = auxm2;
			contPassosM++;//conta passos mosquito
		}
		aux = aux -> down;
	}

}
//impressoes, TRANSFORMAR NA FUNÇAO DE GERAÇAO DE LOG!!
void imprimeligacoes(casaligada *x,FILE *lista){
	if(x == NULL) return;
	if(x -> prox == NULL){
		fprintf(lista,"%s",x -> nome);
		return;
	}
	fprintf(lista,"%s -> ",x -> nome);
	imprimeligacoes(x -> prox, lista);
}

void imprimemosquitos(mosquito *x,FILE *lista){
	if(x == NULL) return;
	if(x -> prox == NULL){
		fprintf(lista,"M");
		return;
	}
	fprintf(lista,"M -> ");
	imprimemosquitos(x -> prox, lista);
}

void imprime(casas *x,FILE *lista){
	if(x == NULL){
		fprintf(lista,"\n");
		return;
	}
	fprintf(lista,"%s -> ",x -> nome);
	imprimeligacoes(x -> cl,lista);
	if(x -> m != NULL){
		fprintf(lista,"-> ");
		imprimemosquitos(x -> m,lista);
	}
	fprintf(lista,"\n");
	imprime(x -> down,lista);
}
void geralista(casas *x,int inifim){
	FILE *lista = fopen("lista.txt","a");
	if(inifim)fprintf(lista,"Inicial:\n");
	else fprintf(lista,"Final:\n");
	imprime(x,lista);
	fclose(lista);
}
//impressao
void desaloca(mosquito **p){

	if(*p == NULL) return ;

	desaloca(&(*p)->prox);
	free(*p);
	*p = NULL;

}

int agente_atua(casas *p, int cont){

	casas *aux = p;
	int n=0;

	n=rand()%cont+1;
	// printf("%d\n",n);
	for(int i=1;i<n;i++) aux = aux->down;
	printf("Casa selecionada agente atua:%s\n",aux -> nome);
	if(aux->m == NULL){
		if(aux -> infected == 1){
			aux -> infected = 0; //CHAMANDO AGNETE_SAUDE
			return 1;
		}
		return 0;
	}
	desaloca(&aux->m);
	return 1;

}

int agente_saude(casas *p, int cont){

	casas *aux = p;
	int n=0;

	n=rand()%cont+1;
	// printf("%d\n",n);
	for(int i=1;i<n;i++) aux = aux->down;

	printf("Casa selecionada agente saude:%s\n",aux -> nome);
	if(aux -> infected == 0) return 0;
	if(aux->m != NULL){
		desaloca(&aux->m); // AGENTE_ATUA
	}
	aux->infected = 0;
	return 1;

}
//geraçao de LOG
void geralog(int minicial, int mfinal, int mmov,int aerro, int aacerto, int epidemia){
	FILE *log = fopen("log.txt", "w");
	if(log == NULL){
		puts("Erro log.txt");
		return;
	}

	fprintf(log,"Numero de mosquitos iniciais: %d\n",minicial);
	fprintf(log,"Numero de mosquitos finais: %d\n",mfinal);
	fprintf(log,"Numero de movimentos totais dos mosquitos: %d\n",mmov);
	fprintf(log,"Numero de erros dos agentes: %d\n",aerro);
	fprintf(log,"Numero de acertos dos agentes: %d\n",aacerto);
	fprintf(log,"Epidemia: ");
	if(epidemia == 1) fprintf(log, "Nao houve surto\n");
	if(epidemia == 2) fprintf(log, "Houve surto mas foi controlado/erradicado\n");
	if(epidemia == 3) fprintf(log, "Estado de emergencia.\n");
	fclose(log);
}

//impressoes, TRANSFORMAR NA FUNÇAO DE GERAÇAO DE LOG!!
void imprimelig(casaligada *x){
	if(x == NULL){
		return;
	}
	printf("%s -> ",x -> nome);
	imprimelig(x -> prox);
}

void imprimemm(mosquito *x){
	if(x == NULL){
		return;
	}
	printf("M -> ");
	imprimemm(x -> prox);
}

void imprimiu(casas *x){
	if(x == NULL){
		printf("\n");
		return;
	}
	printf("%s ",x -> nome);
	printf("pessoas: %i infectado:%i -> ",x -> Npessoas,x -> infected);
	imprimelig(x -> cl);
	imprimemm(x -> m);
	printf("\n");
	imprimiu(x -> down);
}
//impressao

//mosquito bota
void mosquito_bota(casas *x,int n){
	casas *aux = x;
	int np;
	mosquito *auxm;
	while(aux != NULL){
		auxm = aux -> m;
		np = 0;
		while(auxm != NULL){
			if(auxm -> Npassos >= n){
				auxm -> Npassos = 0;
				np++;
				inseremosquito(x,aux -> nome);
				inseremosquito(x,aux -> nome);
			}
			auxm = auxm -> prox;
		}
		aux = aux -> down;
	}
}



int main(){
	//variaveis
	srand(time(NULL));
	char v[5],f[5];
	int n;
	casas *principal = NULL;
	FILE *arq,*lista;
	char doit[50],IDcasa[4],LigaCasa1[4],LigaCasa2[4],CasaMosquito[4];
	int i=1,result,j=0;
	int Nagente_atua,Nagente_saude,NmosquitoBota,Nsimulacoes,Npessoas;//variaveis com n padroes
	int nMoradoresTotal = 0,nIfectadosTot = 0,nPessoas80porcento,chaveSurto = 0,keykey = 1,epidemia = -1; //verifica surto
	int mInicial,mFinal; //log para mosquito
	int nAacertos = 0,nAerros = 0; //log para agentes
	contPassosM = 0;
	//variaveis FIM

	arq = fopen("entrada.txt", "r");
	if(arq == NULL){
		puts("Erro entrada.txt");
		return 0;
	}
	lista = fopen("lista.txt","w");
	fclose(lista);

	while(!feof(arq)){
		if(!strcmp(doit,"FIM")){
			mFinal = contamosquito(principal);
			//geralog(int minicial, int mfinal, int mmov,int aerro, int aacerto, int epidemia)
			geralog(mInicial,mFinal,contPassosM,nAerros,nAacertos,epidemia);
			geralista(principal,0); //gera parte final da lista
			break;
		 }
		else if(!strcmp(doit,"AGENTE_ATUA")){

			fscanf(arq,"%d",&Nagente_atua);
			strcpy(doit,"nada");
		}
		else if(!strcmp(doit,"AGENTE_SAUDE")){
			fscanf(arq,"%d",&Nagente_saude);
			strcpy(doit,"nada");
		}
		else if(!strcmp(doit,"MOSQUITO_BOTA")){
			fscanf(arq,"%d",&NmosquitoBota);
			strcpy(doit,"nada");
		}
		else if(!strcmp(doit,"inserecasa")){
			contcs++;
			fscanf(arq,"%s",IDcasa);
			fscanf(arq,"%d",&Npessoas);
			nMoradoresTotal += Npessoas;
			inserecasa(&principal,IDcasa,Npessoas);
			strcpy(doit,"nada");
		}
		else if(!strcmp(doit,"ligacasas")){

			fscanf(arq,"%s",LigaCasa1);
			fscanf(arq,"%s",LigaCasa2);
			ligacasas(principal,LigaCasa1,LigaCasa2);
			strcpy(doit,"nada");
		}
		else if(!strcmp(doit,"inseremosquito")){

			fscanf(arq,"%s",CasaMosquito);
			inseremosquito(principal,CasaMosquito);
			strcpy(doit,"nada");
		}
		else if(!strcmp(doit,"iniciasimulacao")){
			fscanf(arq,"%d",&Nsimulacoes);
			geralista(principal,1); //gera parte inicial da lista
			mInicial = contamosquito(principal); //conta o numero de mosquitos iniciais
			nIfectadosTot = containfectado(principal); //conta o numero de infectados total inicial
			nPessoas80porcento = nMoradoresTotal * 0.8;
			//verifica a possibilidade de surto antes de iniciar a simulaçao
			//exemplo: se todas as casas ja possuirem mosquitos antes do inicio da simulaçao
			//             nao podera acontecer o caso "nao houve surto", pois a simulaçao ja iniciou com ESTADO DE EMERGENCIA.
			printf("Primeiro teste\n");
			printf("80%: %i infectados:%i\n",nPessoas80porcento,nIfectadosTot);
			imprimiu(principal);
			if(keykey)
				if(nIfectadosTot > nPessoas80porcento){
					chaveSurto = 1; //verifica o surto
					keykey = 0;
				}
			printf("Começo da simulacao: \n");
			for(j=1;j <= Nsimulacoes; j++){
				printf("%iº rodada.\n",j);
				//SIMULATION
				mosquito_move(principal);
				//para verificar o surto
				nIfectadosTot = containfectado(principal); //conta o numero de infetados apos o mmove
				if(keykey)
					if(nIfectadosTot > nPessoas80porcento){
						chaveSurto = 1; //verifica o surto
						keykey = 0;
					}
				//para verificar o surto FIM
				if(j % Nagente_atua == 0 || j % Nagente_saude == 0){
					if(j % Nagente_atua == 0){
						if(agente_atua(principal,contcs)) nAacertos++;
						else nAerros++;
					}
					if(j % Nagente_saude == 0){
						if(agente_saude(principal,contcs)) nAacertos++;
						else nAerros++;
					}
					printf("Nº acertos agente :%i Nºerros agente:%i\n",nAacertos,nAerros);
				}
				printf("80%: %i infectados:%i\n",nPessoas80porcento,nIfectadosTot);
				imprimiu(principal);
				if(!contamosquito(principal)) break;
				mosquito_bota(principal,NmosquitoBota);
			}
			//para verificar o surto
			nIfectadosTot = containfectado(principal); //conta o numero de infetados apos o mmove
			printf("Ultimo teste:\n");
			printf("80%: %i infectados:%i\n",nPessoas80porcento,nIfectadosTot);
			imprimiu(principal);
			if(keykey)
				if(nIfectadosTot > nPessoas80porcento){
					chaveSurto = 1; //verifica o surto
					keykey = 0;
				}
			//para verificar o surto FIM
			strcpy(doit,"nada");
		}
		else if(!strcmp(doit,"verificasurto")){
			if(nIfectadosTot > nPessoas80porcento){
				epidemia = 3;
			}
			else{
				if(chaveSurto){
					epidemia = 2;
				}
				else epidemia = 1;
			}
			strcpy(doit,"nada");
		}


		setbuf(stdin,NULL);
		result = fscanf(arq,"%s",doit);
		// if(result) printf("Linha %d -> %s\n",i,doit);
		i++;
	}
	fclose(arq);
}
