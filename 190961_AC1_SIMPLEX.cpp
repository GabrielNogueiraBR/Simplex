#include <stdio.h>
#include <stdlib.h>

//funcao para retornar o numero de variaveis de decisao do problema
int retorna_vd();

//funcao para retornar o numero de restricoes do problema
int retorna_r();

main(){
	
	//numero de variaveis de decisao
	int vd = retorna_vd();
	
	//numero de restricoes do problema
	int rest = retorna_r();
	
	
	printf("\nNumero de variaveis de decisao: %i", vd);
	printf("\nNumero de restricoes: %i", rest);
	
}

int retorna_vd(){
	
	FILE *fptr = NULL;
	
	int qtde = 0,i,j;
	float col[10];
	
	char aux; // para identificar quando vai pular de linha
		
	if((fptr = fopen("simplex.txt","r")) == NULL){
		printf("\nArquivo inexistente.\n");
	}
	else{
		
		for(i = 0 ; i < 10; i++){
			col[i] = -1;
		}
		
		for(i = 0 ; i < 10; i++){
			fscanf(fptr, "%f%c", &col[i], &aux);
			if(aux == '\n')
				break;
		}
			
		for(j=0; j <10 ; j++){
			if(col[j] != -1 )
			{
				qtde++; //quantidade das variaveis de decisao
			}	
		}	
		fclose(fptr);	
	}
		
	return qtde; // retorna o numero de variaveis de decisao do problema
}


int retorna_r(){
	
	FILE *fptr = NULL;
	int i = 0 ; 	//para separar a primeira linha (Z) das demais (restricoes)
	int qtde = 0; 	//para registrar a quantidade de restricoes
	char aux; 		//para identificar quando vai pular de linha
	float f;
	
	if((fptr = fopen("simplex.txt","r")) == NULL){
		printf("\nArquivo inexistente.\n");
	}
	else{
		while(getc(fptr) != EOF){
			
			fscanf(fptr, "%c",&aux);
			
			if(aux == '\n' && i == 0){
				i++;//apenas para separar a primeira linha do simplex com as restricoes
			}
			
			if(aux == '\n'){
				qtde++;//quantidade de restricoes
				
			}
			
		}
		fclose(fptr);	
	}
		
	return qtde; // retorna o numero de variaveis de decisao do problema
}









