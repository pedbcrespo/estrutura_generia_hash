#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"hashing.h"

struct tabela{
    int id;
    void*item;
};
//=======================TBL FUNCOES===================//
tbl inicializa(int tamanho){
    tbl vetor = (struct tabela*)malloc(tamanho*sizeof(struct tabela));
    for(int i=0; i<tamanho; i++){
        vetor[i].id = 0;
        vetor[i].item = NULL;
    }
    return vetor;
}

tbl destruir(tbl vetor, int tamanho){
    for(int i=0; i<tamanho; i++){
        free(vetor[i].item);
    }
    free(vetor);
    return NULL;
}

//=======================INT FUNCOES===================//
int hash(int valor, int tam){
	return valor%tam;
}
int hash2(int valor, int tam){
	return tam - 2 - (valor % (tam - 2));
}

int buscar(tbl tab, int tamanho_tab, int id, void*objeto, int tam_obj){
    int pos = hash(id, tamanho_tab);
    int desl = hash2(pos, tamanho_tab);

    while(tab[pos].id != id && tab[pos].id != 0){
        pos = (pos+desl) % tamanho_tab;
    }
    if(tab[pos].id == 0){
        return 0;
    }else{
        memcpy(objeto, tab[pos].item, tam_obj);
        return 1;
    }

}

int cheio(tbl vetor, int tamanho){
    int result = 1;
    for(int i=0; i<tamanho; i++){
        result = result && vetor[i].id != 0;
    }
    return result;
}

int inserir_elem(tbl tab, int tamanho_tab, int id, void*objeto, int tam_obj){
    /*verificar a funcao hash
        relembrar como funciona, aparentemente pode nao funcionar direito
    */
    
    int pos = hash(id, tamanho_tab);
    int desl = hash2(pos, tamanho_tab);
    if(cheio(tab, tamanho_tab))
        return 0;

    while(tab[pos].id != 0){
        pos = (pos+desl)%tamanho_tab;
    }
    tab[pos].id = id;
    tab[pos].item = (void*)malloc(tam_obj);
    memcpy(tab[pos].item, objeto, tam_obj);
    return 1;
}

//=======================VOID FUNCOES===================//
void inserir(tbl tab, int tamanho_tab, int id, void*objeto, int tam_obj){
    inserir_elem(tab, tamanho_tab, id, objeto, tam_obj)? printf("elemento inserido\n") : printf("tabela cheia\n");
}

void imprimir(tbl tab, int tamanho){
    for(int i=0; i<tamanho; i++){
        printf("%d\n", tab[i].id);
    }
}

void salvar_arquivo(tbl tab, int tam){//Talvez eu nao use
    FILE*arq = fopen("tabela_hash.bin", "wb");
    fwrite(tab, sizeof(struct tabela), tam, arq);
    fclose(arq);
}