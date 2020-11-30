typedef struct tabela *tbl;

tbl inicializa(int tamanho, int tam_obj);
tbl destruir(tbl vetor);

int buscar(tbl tab, int id, void*objeto, int tam_obj);

void inserir(tbl tab, int id, void*objeto, int tam_obj);
void imprimir(tbl tab);