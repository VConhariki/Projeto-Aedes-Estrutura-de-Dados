#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(){
	srand(time(NULL));
	int n=2,x=0;
	while(x < 20){
		printf("%i\n",rand()%n);
		x++;
	}
}
