#include <stdio.h>
#include <stdlib.h>
#include "fila.h"

struct fila_t *fila_cria () {
    struct fila_t *fila;

    if (!(fila = malloc(sizeof(struct fila_t)))) 
        return NULL;

    fila -> prim = NULL;
    fila -> ult = NULL;
    fila -> num = 0;

    return fila;
}

int fila_tamanho (struct fila_t *f) {
    
    if (!f)
        return -1;
    
    return f -> num;
}

int fila_insere (struct fila_t *f, int item) {
    struct fila_nodo_t *inserido;
    
    if (!f)
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

void fila_imprime (struct fila_t *f) {
    struct fila_nodo_t *p;

    if (!f)
        return;     

    p = f -> prim;
    if (!p) 
        return;

    while (p) {
        printf("%2d ", p -> item);
        p = p -> prox;
    }
}

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