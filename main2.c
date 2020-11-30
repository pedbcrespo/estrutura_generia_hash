#include<stdio.h>
#include<stdlib.h>
#include"hashing.h"

int menu(char*texto);

typedef struct Jogo{
    char titulo[50];
    char empresa[40];
    char genero[15];
    int ano_lancamento;
    float avaliacao;
}jogo;

int main(){
    system("cls");
    int opc =-1, tam = 11, id;
    tbl vetor = inicializa(tam, sizeof(jogo));
    
    while(opc != 0){
        opc = menu("1)inserir\n2)buscar\n3)imprimir\n0)sair\n");
        system("cls");
        if(opc == 1){
            jogo novo;
            printf("id: ");
            scanf("%d", &id);
            printf("titulo: ");
            scanf("%s", novo.titulo);
            printf("empresa: ");
            scanf("%s", novo.empresa);
            printf("ano de lancamento: ");
            scanf("%d", &novo.ano_lancamento);
            printf("genero: ");
            scanf("%s", &novo.genero);
            printf("avaliacao: ");
            scanf("%f", &novo.avaliacao);

            inserir(vetor, id, &novo, sizeof(novo));
        }
        else if(opc == 2){
            int id_busca;
            jogo buscado;

            printf("id: ");
            scanf("%d", &id_busca);

            if(buscar(vetor,id_busca, &buscado, sizeof(buscado))){
                printf("titulo: %s\nempresa: %s\nano: %d\ngenero: %s\navaliacao: %.2f\n", buscado.titulo, buscado.empresa, buscado.ano_lancamento, buscado.genero, buscado.avaliacao);
            }else{
                printf("nao encontrado\n");
            }
        }
        else if(opc == 3){
            imprimir(vetor);
        }
        else{
            break;
        }
        system("pause");
        system("cls");
    }
}

int menu(char*texto){
    int opc;
    printf("%s", texto);
    scanf("%d", &opc);
    return opc;
}