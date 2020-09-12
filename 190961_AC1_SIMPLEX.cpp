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
	
	//numero de variaveis de folga
	int var_folgas = rest;
	
	//numero total de colunas da tabela
	int col = vd + var_folgas + 1; //+1 por causa de Z (funcao)
	
	//numero total de linhas da nossa tabela
	int lin = rest + 1;
	
	
	printf("\nNumero de variaveis de decisao: %i", vd);
	printf("\nNumero de restricoes: %i", rest);
	
}

int retorna_vd(){
	
	FILE *fptr = NULL;
	
	int qtde = 0; //vai retornar o numero de variaveis de decisao
	float col; //vai identificar se a culuna esta preenchida
	char aux; // para identificar quando vai pular de linha
	char fim_arquivo; //para controlar o while e o fim do ponteiro para FILE
		
	if((fptr = fopen("simplex.txt","r")) == NULL){
		printf("\nArquivo inexistente.\n");
	}
	else{
		
		while(fim_arquivo != EOF){
			
			col = -1;
			fscanf(fptr,"%f%c", &col,&aux);
			
			if(col != -1){
				qtde++; //quantidade das variaveis de decisao
			}
			
			if(aux == '\n')
				break; //termina a primeira linha
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
	char fim_arquivo; //para controlar o while e o fim do ponteiro para FILE
	
	if((fptr = fopen("simplex.txt","r")) == NULL){
		printf("\nArquivo inexistente.\n");
	}
	else{
		
		while(aux != EOF){
			
			aux = getc(fptr);
			printf("\n--%c--\n", aux);
			
			if(aux == '\n' && i > 0){
				qtde++;//quantidade de restricoes
			}
			
			i++;//apenas para separar a primeira linha do simplex com as restricoes
		}
		fclose(fptr);
	}
	return qtde; // retorna o numero de variaveis de decisao do problema
}









