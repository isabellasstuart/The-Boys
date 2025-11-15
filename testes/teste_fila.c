#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define MAX 5

/* ---------------------------------------------------------fila.h------------------------------------------------------------------ */

// descreve um nodo da fila 
struct fila_nodo_t
{
	int item;					// item associado ao nodo
	struct fila_nodo_t *prox;	// próximo nodo
};

// descreve uma fila 
struct fila_t
{
	struct fila_nodo_t *prim ;	// primeiro nodo da fila
	struct fila_nodo_t *ult ;	// último nodo da fila
	int num ;					// número de itens na fila
};

/* -------------------------------------------------------- fila.c------------------------------------------------------------------ */

// Cria uma fila vazia.
// Retorno: ponteiro para a fila criada ou NULL se erro.
struct fila_t *fila_cria () {
    struct fila_t *fila;

    if (!(fila = malloc(sizeof(struct fila_t)))) 
        return NULL;

    fila -> prim = NULL;
    fila -> ult = NULL;
    fila -> num = 0;

    return fila;
}

// Informa o número de itens na fila.
// Retorno: N >= 0 ou -1 se erro.
int fila_tamanho (struct fila_t *f) {
    
    if (!f)
        return -1;
    
    return f -> num;
}

// Insere um item no final da fila (politica FIFO).
// Retorno: 1 se tiver sucesso ou 0 se falhar.
int fila_insere (struct fila_t *f, int item) {
    struct fila_nodo_t *inserido;
    
    if (!f)
        return 0;

    if (fila_tamanho(f) >= MAX)    // alterar esse max para lotacao da fila
        return 0;

    if (!(inserido = malloc(sizeof(struct fila_nodo_t))))
        return 0;

    inserido -> item = item;
    inserido -> prox = NULL;
    
    if (!(fila_tamanho(f))) 
        f -> prim = inserido;
    
    else 
        f -> ult -> prox = inserido;

    f -> ult = inserido;
    (f -> num)++;

    return 1;
}

// Retira o primeiro item da fila e o devolve
// Retorno 1 se a operação foi bem sucedida e 0 caso contrário
int fila_retira (struct fila_t *f, int *item) {
    struct fila_nodo_t *removido;

    if (!f)
        return 0;

    if (!(fila_tamanho(f)))
        return 0;

    removido = f -> prim;
    *item = removido -> item;
    f -> prim = removido -> prox;
    
    free(removido);
    removido = NULL;
    (f -> num)--;

    return 1;
}

// Imprime o conteúdo da fila 
void fila_imprime (struct fila_t *f) {
    struct fila_nodo_t *p;

    if (!f)
        return;     // perguntar se é boa prática

    p = f -> prim;
    if (!p) {
        printf("Fila vazia!\n");
        return;
    }

    while (p != f -> ult) {
        printf("%d ", p -> item);
        p = p -> prox;
    }
    printf("%d\n", p -> item);
}

// Libera todas as estruturas de dados da fila, inclusive os itens.
// Retorno: NULL.
struct fila_t *fila_destroi (struct fila_t *f) {
    int removido;

    if (!f)
        return NULL;

    while (fila_tamanho(f))
        fila_retira(f, &removido);

    free(f);
    f = NULL;

    return f;
}


// testando tad fila

/* int main() {
    struct fila_t *f;
    int num;

    // testar se criar fila nao da vazamento de memoria
    f = fila_cria();
    printf("Tamanho pós inicialização: %d\n", fila_tamanho(f));

    // inserir elementos e ver se nao da segfault
    for (int i = 1; i <= 5; i++) {
        if (scanf("%d", &num) != 1)
            return 1;
        fila_insere(f, num);
    }

    // testar se deu certo a insercao e o tamanho novo
    fila_imprime(f);
    printf("Tamanho pós inserção: %d\n", fila_tamanho(f));

    for (int i = 1; i <= 5; i++) {
        fila_retira(f, &num);
        printf("Valor retirado: %d\n", num);
    }

    // testar se deu certo a remocao e o tamanho novo
    fila_imprime(f);
    printf("Tamanho pós remoção: %d\n", fila_tamanho(f));

    if (!fila_retira(f, &num))
        printf("Tentar remover elemento em fila vazia não deu segfault\n");

    fila_destroi(f);

    // tentar destruir uma lista vazia - sem leak e nem segfault
    f = fila_cria();
    fila_destroi(f);

    return 0;
} */
