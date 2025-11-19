#include <stdio.h>
#include <stdlib.h>
#include "fprio.h"

struct fprio_t *fprio_cria () {
    struct fprio_t *fila;

    if (!(fila = malloc(sizeof(struct fprio_t))))
        return NULL;

    fila -> prim = NULL;
    fila -> fim = NULL;
    fila -> num = 0;

    return fila;
}

int fprio_tamanho (struct fprio_t *f) {
    if (f)
        return f -> num;
    return -1;
}

int fprio_insere (struct fprio_t *f, void *item, int tipo, int prio) { 
    struct fpnodo_t *inserido, *p; 

    if ((!f) || (!item))
        return -1;

    if (!(inserido = malloc(sizeof(struct fpnodo_t))))
        return -1;

    // Verifica se o item ja existe na fila 
    p = f -> prim;
    while (p) {
        if (p -> item == item) {
            free(inserido);
            return -1;  
        } 
        p = p -> prox;
    }

    inserido -> item = item;
    inserido -> tipo = tipo;
    inserido -> prio = prio;
    inserido -> prox = NULL;

    // Insere no início
    if (!(fprio_tamanho(f)) || (f -> prim -> prio > prio)) {
        inserido -> prox = f -> prim;
        f -> prim = inserido;
    }
    else {
        p = f -> prim;

        // Encontra valor anterior à posição correta de inserido
        while ((p -> prox) && ((p -> prox -> prio) <= (prio))) 
            p = p -> prox;

        inserido -> prox = p -> prox;
        p -> prox = inserido;
    }

    // Atualiza f->fim se item for inserido no fim da fprio
    if (!(fprio_tamanho(f)) || (f -> fim -> prox))
        f -> fim = inserido;

    (f -> num)++;

    return f -> num;
}

void *fprio_retira (struct fprio_t *f, int *tipo, int *prio) {
    struct fpnodo_t *removido;
    void *item;

    if ((!f) || (!tipo) || (!prio) || !fprio_tamanho(f))
        return NULL;

    removido = f -> prim;
    *tipo = removido -> tipo;
    *prio = removido -> prio;
    item = removido -> item;

    f -> prim = removido -> prox;
    free(removido);
    removido = NULL;
    (f -> num)--;

    return item;
}

void fprio_imprime (struct fprio_t *f) {
    struct fpnodo_t *p;

    if ((f) && (p = f -> prim)) {
        while (p -> prox) {
            printf("(%d %d) ", p -> tipo, p -> prio);
            p = p -> prox;
        }
        printf("(%d %d)", p -> tipo, p -> prio);
    }
}

struct fprio_t *fprio_destroi (struct fprio_t *f) {
    void *item;
    int tipo, prio;

    if (!f)
        return NULL;

    while (fprio_tamanho(f)) {
        item = fprio_retira(f, &tipo, &prio);
        free(item);
    }
    free(f);
    f = NULL;

    return f;
}