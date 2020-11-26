#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
/*Trabalho de Estrurura de dados
*
*Grupo:
*   -Pedro de Barros Crespo
*   -Savio Carvalho Moraes
*   -Leonardo Fernandes
*/

typedef struct aluno{
	char nome[50];
	char curso[30];
	int matricula;//3 conjuntos de 3 digitos Ex.: 123 456 789
}aln;

aln*inicializa(int tam);
aln*ler_arquivo(char*arquivo, int*tam);
aln*inserir(aln*vetor, int tam, int matricula, char*nome, char*curso);
int menu();
int buscar(aln*vetor, int tam, int matricula, int*ret);
int hash(int valor, int tam);
int hash2(int valor, int tam);
int hash_mat(int matricula, int tam);
int tam_vet_arq(char*arquivo);
void imprimir(aln*vetor, int tam);
void salvar_arquivo(aln*vetor, int tam);
void imprime_aluno(char*arquivo, int matricula);

int main(){
	int opc = -1, mat, tam = 11;
	aln*vetor_alunos = ler_arquivo("lista_alunos.bin", &tam);
	if(vetor_alunos == NULL)
		vetor_alunos = inicializa(tam);
	char nome[50], curso[30];

	while(opc != 4){
		system("cls");
		opc = menu();
		system("cls");
		if(opc == 1){
			printf("nome: ");
			scanf("%s", nome);
			printf("curso: ");
			scanf("%s", curso);
			printf("matricula: ");
			scanf("%d", &mat);
			vetor_alunos = inserir(vetor_alunos, tam, mat, nome, curso);
			salvar_arquivo(vetor_alunos, tam);
		}
		else if(opc == 2){
			printf("matricula: ");
			scanf("%d", &mat);
			imprime_aluno("lista_alunos.bin", mat);

		}
		else if(opc == 3){
			imprimir(vetor_alunos, tam);
		}else{
			break;
		}
		system("pause");
	}

}

//=========================ALN FUNCOES=======================//
aln*inicializa(int tam){
	aln*vetor = (aln*)malloc(tam*sizeof(aln));
	for(int i=0; i<tam; i++){
		vetor[i].matricula = -1;
	}
	return vetor;
}
aln*ler_arquivo(char*arquivo, int*tam){
	FILE*arq = fopen(arquivo, "rb");
	if(arq == NULL)
		return NULL;
	*tam = tam_vet_arq(arquivo);
	aln*vetor = (aln*)malloc(*tam*sizeof(aln));
	for(int i=0; i<*tam; i++)
		fread(&vetor[i], sizeof(aln), 1, arq);
	return vetor;
}
aln*inserir(aln*vetor, int tam, int matricula, char*nome, char*curso){
	int pos = hash_mat(matricula, tam);
	int desl = hash2(pos, tam);
	while(vetor[pos].matricula != -1){
		pos = (pos+desl)%tam;
	}
	strcpy(vetor[pos].curso, curso);
	strcpy(vetor[pos].nome, nome);
	vetor[pos].matricula = matricula;
	return vetor;
}

//=========================INT FUNCOES=======================//
int menu(){
	int opc;
	printf("1)Inserir\n2)Imprimir um elemento\n3)Imprimir toda a tabela\n4)sair\n");
	scanf("%d", &opc);
	return opc;
}
int buscar(aln*vetor, int tam, int matricula, int*ret){
	int pos = hash_mat(matricula, tam);
	int desl = hash2(pos, tam);

	while(vetor[pos].matricula != matricula && vetor[pos].matricula != -1)
		pos = (pos+desl) % tam;
	if(vetor[pos].matricula == -1)
		return 0;
	else{
		*ret = pos;
		return 1;
	}
}
int hash(int valor, int tam){
	return valor%tam;
}
int hash2(int valor, int tam){
	return tam - 2 - (valor % (tam - 2));
}
int hash_mat(int matricula, int tam){
	return (matricula%1000)%tam;
}
int tam_vet_arq(char*arquivo){
	FILE*arq = fopen(arquivo, "rb");
	if(arq == NULL)
		return 0;
	fseek(arq, 0, SEEK_END);
	int tam = ftell(arq)/sizeof(aln);
	fclose(arq);
	return tam;
}
int achar_posicao(char*arquivo, int matricula, int tam){
	int pos = hash_mat(matricula, tam);
	int desl = hash2(pos, tam);
	aln a;
	FILE*arq = fopen(arquivo, "rb");
	fseek(arq, pos*sizeof(aln), SEEK_SET);
	fread(&a, sizeof(aln), 1, arq);
	while(a.matricula != -1){
		pos = (pos + desl) % tam;
		fseek(arq, pos*sizeof(aln), SEEK_SET);
		fread(&a, sizeof(aln), 1, arq);
	}
	fclose(arq);
	return pos;
}

//=========================VOID FUNCOES=======================//
void imprimir(aln*vetor, int tam){
	for(int i=0; i<tam; i++){
		if(vetor[i].matricula != -1)
			printf("%d	%s	%s\n", vetor[i].matricula, vetor[i].nome, vetor[i].curso);
		else
			printf("(vazio)\n");
	}
}
void salvar_arquivo(aln*vetor, int tam){
	FILE*arq = fopen("lista_alunos.bin", "wb");
	fwrite(vetor, sizeof(aln), tam, arq);
	fclose(arq);
}
void imprime_aluno(char*arquivo, int matricula){
	FILE*arq = fopen(arquivo, "rb");
	fseek(arq, 0, SEEK_END);
	int tam = ftell(arq)/sizeof(aln);
	fseek(arq, 0, SEEK_SET);

	int pos = hash_mat(matricula, tam);
	int desl = hash2(pos, tam);

	aln elem;
	fseek(arq, pos*sizeof(aln), SEEK_SET);
	fread(&elem, sizeof(aln), 1, arq);
	while(elem.matricula != matricula && elem.matricula != -1){
		pos = (pos + desl) % tam;
		fseek(arq, pos*sizeof(aln), SEEK_SET);
		fread(&elem, sizeof(aln), 1, arq);
	}

	printf("%d	%s	%s\n", elem.matricula, elem.nome, elem.curso);
}