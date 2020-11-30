#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"hashing.h"

typedef struct hash{
    int id;
    void*objeto;
}*hash;

struct tabela{
    int tamanho;
    hash vetor;
};


//=======================TBL FUNCOES===================//
tbl carregar(int tam_obj){
    FILE*arq = fopen("registros.bin", "rb");
    FILE*arq_id = fopen("chaves.bin", "rb");
    
    tbl vetor = (struct tabela*)malloc(sizeof(struct tabela));

    fseek(arq_id, 0, SEEK_END);
    vetor->tamanho = ftell(arq_id)/sizeof(int);
    fseek(arq_id, 0, SEEK_SET);

    vetor->vetor = (struct hash*)malloc(vetor->tamanho * sizeof(struct hash));

    for(int i=0; i<vetor->tamanho; i++){
        fread(&vetor->vetor[i].id, sizeof(int), 1, arq_id);
        vetor->vetor[i].objeto = (void*)malloc(tam_obj);
        if(vetor->vetor[i].id != 0){
            fread(vetor->vetor[i].objeto, tam_obj, 1, arq);
        }else
            vetor->vetor[i].objeto = NULL;
    }    
    fclose(arq);
    fclose(arq_id);

    return vetor;
}
tbl inicializa(int tamanho, int tam_obj){
    FILE*arq = fopen("registros.bin", "rb");
    FILE*arq_id = fopen("chaves.bin", "rb");
    
    if(arq == NULL && arq_id == NULL){
        tbl vetor = (struct tabela*)malloc(sizeof(struct tabela));
        vetor->tamanho = tamanho;
        vetor->vetor = (struct hash*)malloc(tamanho*sizeof(struct hash));
        for(int i=0; i<tamanho; i++){
            vetor->vetor[i].id = 0;
            vetor->vetor[i].objeto = NULL;
        }        
        return vetor;
    }
    else{
        printf("arquivo .bin existente\n");
        fclose(arq);
        fclose(arq_id);
        return carregar(tam_obj);
    }
}
tbl destruir(tbl vetor){
    for(int i=0; i<vetor->tamanho; i++){
        free(vetor->vetor[i].objeto);
    }
    free(vetor->vetor);
    free(vetor);
    return NULL;
}


//=======================INT FUNCOES===================//
int func_hash(int valor, int tam){
	return valor%tam;
}
int func_hash2(int valor, int tam){
	return tam - 2 - (valor % (tam - 2));
}
int cheio(tbl tab){
    for(int i=0; i<tab->tamanho; i++){
        if(tab->vetor[i].id == 0){
            return 0;
        }
    }
    return 1;
}
int inserir_elem(tbl tab, int id, void*objeto, int tam_obj){
    int pos = func_hash(id, tab->tamanho);
    int desl = func_hash2(pos, tab->tamanho);
    if(cheio(tab)){
        return 0;
    }
    while(tab->vetor[pos].id != 0){
        pos = (pos+desl)%tab->tamanho;
    }
    tab->vetor[pos].id = id;
    tab->vetor[pos].objeto = (void*)malloc(tam_obj);
    memcpy(tab->vetor[pos].objeto, objeto, tam_obj);
    return 1;
}
int buscar(tbl tab, int id, void*objeto, int tam_obj){
    int pos = func_hash(id, tab->tamanho);
    int desl = func_hash2(pos, tab->tamanho);

    while(tab->vetor[pos].id != id && tab->vetor[pos].id != 0){
        pos = (pos+desl) % tab->tamanho;
    }
    if(tab->vetor[pos].id == 0){
        return 0;
    }
    else{
        memcpy(objeto, tab->vetor[pos].objeto, tam_obj);
        return 1;
    }
}


//=======================VOID FUNCOES===================//
void salvar(tbl tab, int tam_obj){
    int*vetor_id = (int*)malloc(tab->tamanho*sizeof(int));
    
    FILE*arq = fopen("registros.bin", "wb");
    FILE*arq2 = fopen("chaves.bin", "wb");

    for(int i=0; i<tab->tamanho; i++){
        vetor_id[i] = tab->vetor[i].id;
        fwrite(tab->vetor[i].objeto, tam_obj, 1, arq);
    }
    fwrite(vetor_id, sizeof(int), tab->tamanho, arq2);
    fclose(arq);
    fclose(arq2);
}
void inserir(tbl tab, int id, void*objeto, int tam_obj){
    if(inserir_elem(tab, id, objeto, tam_obj)){
        salvar(tab, tam_obj);
        printf("valor inserido\n");
    }else{
        printf("tabela cheia\n");
    }

}
void imprimir(tbl tab){
    for(int i=0; i<tab->tamanho; i++){
        printf("%d\n", tab->vetor[i].id);
    }
}
