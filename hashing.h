typedef struct tabela *tbl;

tbl inicializa(int tamanho, int tam_obj);
tbl destruir(tbl vetor);

int buscar(tbl tab, int id, void*objeto, int tam_obj);

void inserir(tbl tab, int id, void*objeto, int tam_obj);
void imprimir(tbl tab);

/*Para rodar o codigo:
    -abra o terminal (cmd caso seja o windows)
    -acesse a pasta do projeto pelo terminal
    -digite (caso queira rodar a main1.c): gcc main1.c hashing.c -o main1.exex
    -digite (caso queira rodar a main2.c): gcc main2.c hashing.c -o main2.exe
    -caso queira rodar seu proprio arquivo.c: gcc [nome_do_seu_arquivo].c hashing.c -o [nome_de_saida].exe
        -ps: lembrar de importar no seu arquivo, a biblioteca "hashing.h"
*/