#include<stdio.h>
#include<stdlib.h>
#include"hashing.h"

int menu(char*texto);

typedef struct Pessoa{
    char nome[50];
    int ano_nascimento;
    float altura;
}pessoa;

int main(){
    system("cls");
    int opc =-1, tam = 11, id;
    tbl vetor = inicializa(tam);
    
    while(opc != 0){
        opc = menu("1)inserir\n2)buscar\n3)imprimir\n0)sair\n");
        system("cls");
        if(opc == 1){//Inserir
            pessoa novo;
            printf("id: ");
            scanf("%d", &id);
            printf("nome: ");
            scanf("%s", novo.nome);
            printf("ano de nascimento: ");
            scanf("%d", &novo.ano_nascimento);
            printf("altura: ");
            scanf("%f", &novo.altura);

            inserir(vetor, tam, id, &novo, sizeof(novo));
        }
        else if(opc == 2){//Buscar
            int id_busca;
            pessoa individuo;
            printf("id: ");
            scanf("%d", &id_busca);
            if(buscar(vetor, tam, id_busca, &individuo, sizeof(individuo))){
                printf("nome: %s\nano de nascimento: %d\naltura: %.2f\n", individuo.nome, individuo.ano_nascimento, individuo.altura);
            }else{
                printf("nao encontrado\n");
            }

        }
        else if(opc == 3){//Imprimir
            imprimir(vetor, tam);
        }
        else{//Sair
            break;
        }
        system("pause");
        system("cls");
    }
    vetor = destruir(vetor, tam);
}

int menu(char*texto){
    int opc;
    printf("%s", texto);
    scanf("%d", &opc);
    return opc;
}