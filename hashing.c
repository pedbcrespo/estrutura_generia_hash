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
    /*Essa funcao tem a finalidade de incializar o vetor da tabela Hash
        Inicialmente eh feito uma alocacao dinamica do tamanho na qual eh
        fornecido pelo usuario, e a partir disso, os elementos do vetor sao
        inicializados com 0 e NULL

        A ideia eh que o valor do ID, seja sempre diferente de 0, logo, caso,
        em alguma situacao em que se eh necessario percorrer o vetor, encontre
        o valor 0, signifique o elemento esta vazio e disponivel para armazenar
        algum dado.
    */
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
    /*Ao receber um valor, que no caso sera um valor unico para representar o elemento como o id, por exemplo,
    eh retornado o resto da divisao dele pelo tam, que no caso eh o tamanho do vetor.
    A ideia eh que o resultado, gere uma posicao no vetor, de forma a espalhar bem os dados.
    */
	return valor%tam;
}
int hash2(int valor, int tam){
    /*Funciona parecido com o hash(), porem essa funcao eh chamada para os casos em que o a posicao gerada na
    hash() ja esteja ocupada, sendo necessario gerar uma outra posicao.*/
	return tam - 2 - (valor % (tam - 2));
}

int buscar(tbl tab, int tamanho_tab, int id, void*objeto, int tam_obj){
    /*O procedimento para buscar eh semelhante ao de inserir.
    Como a funcao hash gera a posicao ideal com base no id do objeto, entao para buscar o dado, basta seguir
    pelo mesmo caminho, ou seja, se fornecer o id do objeto desejado, sera gerado a posicao em que ele esta armazenado
    e caso nao esteja, o valor da nova posicao tambem sera o mesmo.
    
    Ou seja, a hash() refaz todo o caminho ate o objeto, se ele existir no vetor
    */
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
    /*Funcao que uso para verificar se o vetor esta com todos os elementos ocupados
    impossibilitando o armazenamento de novos objetos*/
    int result = 1;
    for(int i=0; i<tamanho; i++){
        result = result && vetor[i].id != 0;
    }
    return result;
}

int inserir_elem(tbl tab, int tamanho_tab, int id, void*objeto, int tam_obj){
    /*Com a funcao hash() eh gerado uma posicao ideal com base no id do objeto, com isso 
    se a posicao estiver livre o objeto eh armazenado, caso nao, eh buscado uma outra posicao com
    a funcao hash2()
    
    O parametro objeto eh um ponteiro do tipo void, o que quer dizer que pode assumir qualquer tipo,
    dependendo da necessidade. Porem, para especificar o tipo, eh necessario usar a variavel tam_obj,
    que armazenara o tamanho do tipo da variavel.

    Ou seja, o void*objeto recebera o endereco da variavel (ou estrutura) que se deseja manipular, enquanto
    que o tam_obj recebera o tamanho do tipo do objeto.

    A funcao memcpy copia uma determinada quantidade (o tamanho eh fornecido pelo programador) de bytes e armazena
    em determinado ponteiro. Com isso eh possivel armazenar qualquer tipo de estrutura ou variavel na tabela.
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