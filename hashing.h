/*Aqui fica apenas o necessario*/
typedef struct tabela *tbl;

tbl inicializa(int tamanho);
tbl destruir(tbl vetor, int tamanho);

int buscar(tbl tab, int tamanho_tab, int id, void*objeto, int tam_obj);

void inserir(tbl tab, int tamanho_tab, int id, void*objeto, int tam_obj);
void imprimir(tbl tab, int tamanho);
