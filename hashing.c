#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"hashing.h"

typedef struct tupla{
    int id;
    int pos_registros;
}tup;

struct tabela{
    int tamanho;
    tup*vetor;
};


//=======================TBL FUNCOES===================//
tbl carregar(int tam_obj){
    FILE*chaves = fopen("chaves.bin", "rb");
    
    if(chaves == NULL){
        return NULL;
    }

    tbl tab = (struct tabela*)malloc(sizeof(struct tabela));
    fseek(chaves, 0, SEEK_END);
    tab->tamanho = ftell(chaves)/sizeof(tup);
    fseek(chaves, 0, SEEK_SET);

    tab->vetor = (tup*)malloc(tab->tamanho * sizeof(tup));
    fread(tab->vetor, sizeof(tup), tab->tamanho, chaves);
    fclose(chaves);
    return tab;
}

tbl inicializa(int tamanho, int tam_obj){
    tbl tab = carregar(tam_obj);
    if(tab == NULL){
        printf("arquivos .bin inexistentes\n");
        FILE*chaves = fopen("chaves.bin", "wb");
        FILE*registros = fopen("registros.bin", "wb");
        fclose(registros);
        tab = (struct tabela*)malloc(sizeof(struct tabela));
        tab->tamanho = tamanho;
        tab->vetor = (tup*)malloc(tamanho*sizeof(tup));
        for(int i=0; i< tab->tamanho; i++){
            tab->vetor[i].id = 0;
            tab->vetor[i].pos_registros = -1;
        }

        fwrite(tab->vetor, sizeof(tup), tamanho, chaves);
        fclose(chaves);
    }
    return tab;
}

tbl destruir(tbl tab){
    free(tab->vetor);
    free(tab);
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

int buscar(tbl tab, int id, void*objeto, int tam_obj){
    FILE*chaves = fopen("chaves.bin", "rb");
    int pos = func_hash(id, tab->tamanho);
    int desl = func_hash2(pos, tab->tamanho);
    tup aux;    
    fseek(chaves, pos*sizeof(tup), SEEK_SET);
    fread(&aux, sizeof(tup), 1, chaves);
    while(aux.id != 0 && aux.id != id){
        pos = (pos + desl) % tab->tamanho;
        fseek(chaves, pos*sizeof(tup), SEEK_SET);
        fread(&aux, sizeof(tup), 1, chaves);
    }
    fclose(chaves);

    if(aux.id == 0)
        return 0;
    else{
        FILE*registros = fopen("registros.bin", "rb");
        fseek(registros, aux.pos_registros*tam_obj, SEEK_SET);
        fread(objeto, tam_obj, 1, registros);
    }
    return 1;
}

int inserir_elemento(tbl tab, int id, void*objeto, int tam_obj){
    int pos = func_hash(id, tab->tamanho);
    int desl = func_hash2(pos, tab->tamanho);
    if(cheio(tab)){
        return 0;
    }

    while(tab->vetor[pos].id != 0){
        pos = (pos + desl) % tab->tamanho;
    }
    tab->vetor[pos].id = id;
    FILE*registros = fopen("registros.bin", "rb");
    fseek(registros, 0, SEEK_END);
    tab->vetor[pos].pos_registros = ftell(registros)/tam_obj;
    fseek(registros, 0, SEEK_SET);
    fclose(registros);
    return 1;
}



//=======================VOID FUNCOES===================//
void salvar(tbl tab, int id, void*objeto, int tam_obj){
    FILE*chaves = fopen("chaves.bin", "wb");
    FILE*registros = fopen("registros.bin", "ab");

    fwrite(objeto, tam_obj, 1, registros);
    fwrite(tab->vetor, sizeof(tup), tab->tamanho, chaves);
    fclose(registros);
    fclose(chaves);
}

void inserir(tbl tab, int id, void*objeto, int tam_obj){
        void*aux = (void*)malloc(tam_obj);
        if(buscar(tab, id, aux, tam_obj)){
            printf("id ja existente\n");
        }
        else{
            if(inserir_elemento(tab, id, objeto, tam_obj)){
            salvar(tab, id, objeto, tam_obj);
            printf("inserido com sucesso\n");
        }
        else{
            printf("nao foi possivel inserir o elemento\n");
        }
        free(aux);
    }
}

void imprimir(tbl tab){
    for(int i=0; i<tab->tamanho; i++){
        printf("%d\n", tab->vetor[i].id);
    }

    printf("\n\n\n");
}