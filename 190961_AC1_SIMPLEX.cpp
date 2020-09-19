#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//funcao para retornar o numero de variaveis de decisao do problema
int retorna_vd();

//funcao para retornar o numero de restricoes do problema
int retorna_r();

//alocacao de ponteiro para pornteiro de char
void aloca_pchar(char ***p, int tam);

//inserindo os nomes das variaveis
void alimenta_nomes(char **p, int tam);

//alocacao de char
void aloca_char(char **p, int tam);

//mostrando as informacoes da tabela
void mostra_tabela(float *m, int lin, int col, char **p);

//obtendo as informacoes do arquivo e passando para a tabela
void informacoes_arq(float *m, int lin, int col, int vd, int res);

//verifica se a solucao e otima
bool verifica_solucao(float *m, int lin, int col);

//verifica a coluna que vai entrar na base e retorna o indice da coluna
int entra_base(float *m, int lin, int col);

//** 
//verifica a linha que vai sair da base e retorna o indice da linha
int sai_base(float *m, int lin, int col, int coluna_entra);

//troca o nome das variaveis
void troca_nome(char **p, int coluna_entra, int linha_sai, int var_dec);

//recalcula as linhas
void recalcular_linhas(float *m, int lin, int col, int linha_pivo, int coluna_pivo);


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
	
	//numero total de variaveis
	int var_t = vd+rest;
	
	//vetor com o nome das variaveis
	char **x;
	x = NULL;
	
	//alocacao do ponteiro
	aloca_pchar(&x,var_t);
	
	//nome das iniciais das variaveis
	alimenta_nomes(x,var_t);
	
	//vetor para a tabela
	float m[lin*col];
	
	//preenchendo a tabela com as informacoes do arquivo
	informacoes_arq(m,lin,col,vd,rest);
	
	//verificando a solucao otima
	bool solucao_otima = verifica_solucao(m,lin,col);
	
	while(solucao_otima != true){
		
		//mostrando a tabela
		printf("\n\n");
		mostra_tabela(m,lin,col,x);
		
		//indice da coluna que vai entrar na base
		int entra = entra_base(m, lin, col);
		
		//indica a linha que vai sair da base
		int sai = sai_base(m, lin, col, entra);
		
		//atualizar o nome da coluna que sai
		troca_nome(x, entra, sai, vd + 1);
		
		//atualizando as linhas abaixo da pivo
		recalcular_linhas(m, lin, col, sai, entra);
		
		solucao_otima = verifica_solucao(m,lin,col);
		
	}//continua executando enquanto nao encontrar a solucao otima
	
	//mostrando a tabela final
	printf("\n\n");
	mostra_tabela(m,lin,col,x);
	
	
	printf("\n\n\n\n\n\n\t\tINFORMACOES IMPORTANTES:\n");
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
			
			if(aux == '\n' && i > 0){
				qtde++;//quantidade de restricoes
			}
			
			i++;//apenas para separar a primeira linha do simplex com as restricoes
		}
		fclose(fptr);
	}
	return qtde; // retorna o numero de variaveis de decisao do problema
}

void aloca_pchar(char ***p, int tam){
	if( (*p = (char**) realloc(*p, tam*sizeof(char*))) == NULL)
		exit(1);
		
		// p -> &x
		// *p-> valor em x
		//**p-> aponta para um ponteiro
		//***p-> valor do ponteiro final 
}

void alimenta_nomes(char **p, int tam){
	
	//   p -> endereco que esta em x
	//  *p -> ponteiro dentro do endero que esta em x
	// **p -> valor
		
	int i, tam_num;
	char num_str[10];
	
	for(i = 0; i < tam ; i++){
		
		//definir o end inicial como NULL
		*(p+i) = NULL;
		
		//converte int em char
		sprintf(num_str, "x%i", i + 1);
		
		//printf("\nnum  = ");
		//puts(num_str);
		
		tam_num = strlen(num_str) + 1;	
		aloca_char( p+i, tam_num);
		strcpy( *(p+i) , num_str);
	}
}

void aloca_char(char **p, int tam){
	if( (*p = (char*) realloc(*p, tam*sizeof(char))) ==NULL )
		exit(1);
}

void mostra_tabela(float *m, int lin, int col, char **p){
	
	int i, j, k, h;
	int pos; //para andar pelo vetor que representa a tabela 
	k = 1; //apenas para controlar a exibicao das variaveis superiores
	h = 2; //apenas para controlar a exibicao das variaveis laterais
	
	for(i = 0; i < lin + 1 ; i++){
		
		
		// Para exibir os textos superiores (fixo)
		if(i == 0){
			for(j=0; j< col + 1; j++){
				if(j == 0){
					printf("Base");
				}
				else if(k < col){
					printf("\t%s",*(p+k-1));
					k++;
				}
				else if(k == col){
					printf("\tb\n");
				}
			}
		}
		
		// Para exibir as informacoes (variavel)
		else{
			pos = (i-1) * col;
			for(j=0; j< col + 1; j++){
				
				if(j == 0 && i < lin ){
					printf("%s", *(p+h));
					h++;
				}
				
				else if(j== 0 && i == lin){
					printf("Z");
				}
				
				else{
					
					printf("\t%.1f", *(m+pos));
					pos++;
									
					if(j == col){
						printf("\n"); //pulando linha no final da tabela	
					}
				}
			}
		}		
	}
}

void informacoes_arq(float *m, int lin, int col, int vd, int res){
	
	FILE *fptr = NULL;
	
	int linha = -1; //indica a primeira linha
	char aux; // para identificar quando vai pular de linha e o final do arquivo
	float m_aux[lin][col], value; //valores da tabela a serem preenchidos
	int i,j;
	int pos; //para andar com o vetor
	
	//inicializando a matriz com zeros
	for(i = 0; i < lin; i++)
		for(j = 0; j < col; j++)
			m_aux[i][j] = 0;
	
	if((fptr = fopen("simplex.txt","r")) == NULL){
		printf("\nArquivo inexistente.\n");
	}
	
	else{
		j = 0;
		while( aux != EOF){
			
			//recebendo os valores
			fscanf(fptr,"%f", &value);
			aux = getc(fptr);
		
			if(linha == -1){
				//printf("\nA\n");
				m_aux[lin-1][j] = -value;
				j++;
			} //valores da primeira linha
			else{
				
				if(j < vd){
					m_aux[linha][j] = value;
				}
				else if( j == vd )
				{
					m_aux[linha][col-1] = value;
				}
				j++;
			} //valores das demais linhas
						
			if(aux == '\n'){
				linha++; //informar que passou para a proxima linha
				j = 0; //posicao das colunas
			}
			
		}
		fclose(fptr);
		
		//adicionando as variaveis de folga
		for(i = 0; i < lin -1 ; i++)
			for(j = 0; j < col; j++)
			{
				m_aux[i][vd+i] = 1;
			}
		
//		printf("\n\n\n");
//		for(i = 0; i < lin; i++){
//			for(j = 0; j < col; j++)
//				printf("\t%.1f", m_aux[i][j]);
//				
//			printf("\n");
//		}

		//Preenchimento da tabela a partir da matriz
		for(i = 0; i < lin ; i++){
		
			for(j = 0; j < col; j++)
			{
				pos = (i *col) + j; //para encontrar a posicao equivalente no vetor
				
				*(m+pos) = m_aux[i][j];
			}
		}		
	}
}

bool verifica_solucao(float *m, int lin, int col){
	
	bool verifica = true;
	int pos;
	int i;
	
	pos = (lin-1) * col; //definindo o primeiro numero da linha Z (última linha) 
	
	for(i = 0 ; i < col ; i++){
		if( *(m+pos+i) < 0 )
			verifica = false;
	}
		
	return verifica;
}

int entra_base(float *m, int lin, int col){
	
	int maior_negativo = 0;
	int pos;
	int i;
	int entra;
	
	pos = (lin-1) * col; //definindo o primeiro numero da linha Z (última linha) 
	
	for(i = 0 ; i < col ; i++){
		if( *(m+pos+i) < 0 && *(m+pos+i) < maior_negativo){
			maior_negativo = *(m+pos+i); //maior numero negativo
			entra = i; //indice da posicao com o maior numero negativo
		}
			
	}
		
	return entra;
}


