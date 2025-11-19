#include <stdio.h>           
#include <stdlib.h>
#include <stdbool.h>
#include "mundo.h"
#include "eventos.h"  
#include "funcoes.h"

void destroi_herois(struct heroi_t **h, int tam) {  
    int i;

    if (!h)
        return;

    for (i = 0; i < tam; i++)
        if (h[i]) {
            if (h[i] -> habilidades)
                cjto_destroi(h[i] -> habilidades);
                
            free(h[i]);
            h[i] = NULL;
        }
    
    free(h);
}

void destroi_bases(struct base_t **b, int tam) {  
    int i;

    if (!b)
        return;

    for (i = 0; i < tam; i++)
        if (b[i]) {
            if (b[i] -> presentes)
                cjto_destroi(b[i] -> presentes);

            if (b[i] -> espera)
                fila_destroi(b[i] -> espera);

            free(b[i]);
            b[i] = NULL;
        }

    free(b);
}

void destroi_missoes(struct missao_t **m, int tam) {  
    int i;

    if (!m)
        return;

    for (i = 0; i < tam; i++)
        if (m[i]) {
            if (m[i] -> habilidades)
                cjto_destroi(m[i] -> habilidades);

            free(m[i]);
            m[i] = NULL;
        }

    free(m);
}

void destroi_mundo(struct mundo_t **m) {
    if (!m)
        return;
    
    if ((*m) -> lef) 
        (*m) -> lef = fprio_destroi((*m) -> lef);

    destroi_herois((*m) -> herois, (*m) -> n_herois);
    destroi_bases((*m) -> bases, (*m) -> n_bases);
    destroi_missoes((*m) -> missoes, (*m) -> n_missoes);
    free(*m);
    *m = NULL;
}

struct heroi_t *cria_heroi(int *id) {
    struct heroi_t *h;

    if (!(h = malloc(sizeof(struct heroi_t))))
        return NULL;

    h -> id = (*id);
    h -> xp = 0;
    h -> base = -1;
    h -> paciencia = sorteia_num(0, 100);
    h -> velocidade = sorteia_num(50, 5000);
    h -> vivo = 1;
    h -> habilidades = cjto_aleat(sorteia_num(1, 3), N_HABILIDADES);

    if (!(h -> habilidades))
        return NULL;

    (*id)++;

    return h;
}

struct heroi_t **inic_herois() {
    struct heroi_t **v;
    struct heroi_t *h;
    int i, id = 0;

    if (!(v = malloc(sizeof(struct heroi_t *) * N_HEROIS)))
        return NULL;

    for (i = 0; i < N_HEROIS; i++) {
        if (!(h = cria_heroi(&id))) {
            destroi_herois(v, i);
            return NULL;
        }

        v[i] = h;
    }

    return v;
}

struct base_t *cria_base(int *id) {
    struct base_t *b;

    if (!(b = malloc(sizeof(struct base_t))))
        return NULL;

    b -> id = (*id);
    b -> fila_max = 0;
    b -> missoes_cumpridas = 0;
    b -> local = sorteia_coord(0, N_TAMANHO_MUNDO - 1);
    b -> lotacao = sorteia_num(3, 10);
    b -> presentes = cjto_cria(N_HEROIS);
    b -> espera = fila_cria();

    if (!(b -> presentes) || !(b -> espera)) {
        if (b -> presentes)
            cjto_destroi(b -> presentes);
        if (b -> espera)
            fila_destroi(b -> espera);
        return NULL;
    }

    (*id)++;

    return b;
}

struct base_t **inic_bases() {
    struct base_t **v;
    struct base_t *b;
    int i, id = 0;

    if (!(v = malloc(sizeof(struct base_t *) * N_BASES)))
        return NULL;

    for (i = 0; i < N_BASES; i++) {
        if (!(b = cria_base(&id))) {
            destroi_bases(v, i);
            return NULL;
        }

        v[i] = b;
    }
    
    return v;
}

struct missao_t *cria_missao(int *id) {
    struct missao_t *m;

    if (!(m = malloc(sizeof(struct missao_t))))
        return NULL;

    m -> id = (*id);
    m -> cumprida = 0;
    m -> tentativas = 0;
    m -> local = sorteia_coord(0, N_TAMANHO_MUNDO - 1);
    m -> habilidades = cjto_aleat(sorteia_num(6, 10), N_HABILIDADES);

    if (!(m -> habilidades))
        return NULL;

    (*id)++;

    return m;
}

struct missao_t **inic_missoes() {
    struct missao_t **v;
    struct missao_t *m;
    int i, id = 0;

    if (!(v = malloc(sizeof(struct missao_t *) * N_MISSOES)))
        return NULL;

    for (i = 0; i < N_MISSOES; i++) {
        if (!(m = cria_missao(&id))) {
            destroi_missoes(v, i);
            return NULL;
        }

        v[i] = m;
    }
    
    return v;
}

struct mundo_t *cria_mundo() {
    struct mundo_t *mundo;
    struct evento_t *fim;

    if (!(mundo = malloc(sizeof(struct mundo_t))))
        return NULL;

    if (!(fim = malloc(sizeof(struct evento_t))))
        return NULL;

    mundo -> n_herois = N_HEROIS;
    mundo -> n_bases = N_BASES;
    mundo -> n_missoes = N_MISSOES;
    mundo -> n_habilidades = N_HABILIDADES;
    mundo -> n_compostos_v = N_COMPOSTOS_V;
    mundo -> n_eventos = 0;
    mundo -> n_mortos = 0;
    mundo -> missoes_cumpridas = 0;
    mundo -> missoes_min_tent = 1;
    mundo -> missoes_max_tent = 0;
    mundo -> relogio = 0;
    mundo -> lef = fprio_cria();
    mundo -> herois = inic_herois();
    mundo -> bases = inic_bases();
    mundo -> missoes = inic_missoes();
    mundo -> tamanho_mundo = cria_coord(N_TAMANHO_MUNDO, N_TAMANHO_MUNDO);

    if (!(mundo -> lef) || !(mundo -> herois) || !(mundo -> bases) || !(mundo -> missoes))
        return NULL;

    evento_cria(mundo, fim, FIM, T_FIM_DO_MUNDO, NULL, NULL);

    return mundo;
}

void imprime_herois(struct heroi_t **h) {
    int i;

    for (i = 0; i < N_HEROIS; i++) {
        printf("\n");
        printf("id: %d\n", h[i] -> id);
        printf("xp: %d; base: %d; paciencia: %d; velocidade: %d; n_habilidades: %d\n", h[i] -> xp, h[i] -> base, h[i] -> paciencia, h[i] -> velocidade, cjto_card(h[i] -> habilidades));
        printf("habilidades: ");
        cjto_imprime(h[i] -> habilidades);
        printf("\n");
        if (h[i] -> vivo)
            printf("status: vivo\n");
        else   
            printf("status: morto\n");
        printf("\n");
    }
    printf("\n");
}

void imprime_bases(struct base_t **b) {
    int i;

    for (i = 0; i < N_BASES; i++) {
        printf("\n");
        printf("id: %d\n", b[i] -> id);
        printf("lotacao: %d; n_presentes: %d; n_esperando: %d; local: %d x %d\n", b[i] -> lotacao, cjto_card(b[i] -> presentes), fila_tamanho(b[i] -> espera), (b[i] -> local).x, (b[i] -> local).y);
    }
    printf("\n");
}

void imprime_missoes(struct missao_t **m) {
    int i;

    for (i = 0; i < N_MISSOES; i++) {
        printf("\n");
        printf("id: %d\n", m[i] -> id);
        printf("n_habilidades: %d; local: %d x %d\n", cjto_card(m[i] -> habilidades), (m[i] -> local).x, (m[i] -> local).y);
        printf("habilidades: ");
        cjto_imprime(m[i] -> habilidades);
        printf("\n");
    }
    printf("\n");
}

void configura_relogio(struct mundo_t *mundo, int tempo) {
    mundo -> relogio = tempo;
}