#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"hashing.h"

/*  A tabela hash, funcionará usando duas estruturas.
A primeira é tupla que conterá o identinficador do objeto e a posição que ele terá no arquivo "registros.bin".

    Na segunda, teremos o tamanho do vetor que queremos manipular e o vetor de tuplas. Essa será a tabela que o usuario
ira manipular no decorrer do programa.

    O vetor de tuplas será armazenado no arquivo "chaves.bin". Será neste arquivo que as manipulações serão feitas, ou 
seja, as operações de busca e inserção. 
*/
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


/*  A função inicializa vai, inicialmente, tentar carregar o arquivo binario "chaves.bin", caso exista, uma tabela é instanciada e o seus dados serao carregados para a tabela. 
Caso o contrario, uma nova tabela é instanciada e valores iniciais são atribuidos.
    Os valores iniciais 0 e -1 servem para representar que os elementos da tabela estão livres para serem usados.
*/
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

/*O procedimento para destruir a tabela é de inicialmente liberar a área alocada para o vetor de tuplas e logo depois
liberar a área alocada para a tabela*/
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

/*  A função de busca usará as funções de hash para achar a posição ideal que o objeto teria caso fosse inserido.
caso encontre um objeto diferente do ocupado, a posição ideal é recalculada e então é verificado a nova posição até que o
objeto procurado seja encontrado.
Caso encontre um identificador = 0, significa que o valor buscado não existe.
    
    Toda a operação é feita no arquivo "chaves.bin", tendo em vista que a busca é feita pelo identificador. 
Caso seja encontrado o objeto procurado, pela tupla (id, posição), os dados sao buscados no arquivo "registros.bin"
e retornado.

*/

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

/*
    A função calcula a posição ideal para o armazenamento de um determinado valor a partir do identificador.
Caso a posição encontrada já tenha algum elemento armazenado, é calculado uma nova posição até que seja encontrado
um elemento vazio.
    Após isso, o id é armazenado, e a posição no arquivo "registros.bin" é calculada para que no fim seja registrado 
na tupla (id, posição). Em seguida o objeto é armazenado no arquivo "registros.bin".
    No final, é retornado o valor 1, mostrando que o elemento foi inserido com sucesso. Teria retornado o valor 0 caso
não houvesse espaço na tabela, sinalizando que a mesma ja esta cheia.
*/
int inserir_elemento(tbl tab, int id, int tam_obj){
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

/*  Apos o procedimento de inserção, é chamada a função salvar, na qual sobreescreve o conteudo do arquivo "chaves.bin"
pelos dados atualizados da tabela e adiciona no final do arquivo "registros.bin" o objeto inserido
*/
void salvar(tbl tab, int id, void*objeto, int tam_obj){
    FILE*chaves = fopen("chaves.bin", "wb");
    FILE*registros = fopen("registros.bin", "ab");

    fwrite(objeto, tam_obj, 1, registros);
    fwrite(tab->vetor, sizeof(tup), tab->tamanho, chaves);
    fclose(registros);
    fclose(chaves);
}


/*
    Aqui esta implementado a função que o usuario vai ultilizar. A diferença entre essa função e a inseir_elemento, é
que essa realiza alguns procedimentos antes de inserir, como verificar se o id ja existe na tabela, caso seja o procedimento de inserção é interrompido.
    Caso o procedimento de inserção tenha sido um sucesso, a tabela é salva no arquivo "chaves.bin", deixando assim os
dados atualizados.
*/
void inserir(tbl tab, int id, void*objeto, int tam_obj){
        void*aux = (void*)malloc(tam_obj);
        if(buscar(tab, id, aux, tam_obj)){
            printf("id ja existente\n");
        }
        else{
            if(inserir_elemento(tab, id, tam_obj)){
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