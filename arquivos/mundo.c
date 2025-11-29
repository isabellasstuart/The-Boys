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

    for (i = 0; i < tam; i++) {
        if (h[i]) {
            if (h[i] -> habilidades)
                cjto_destroi(h[i] -> habilidades);
                
            free(h[i]);
            h[i] = NULL;
        }
    }
    
    free(h);
}

void destroi_bases(struct base_t **b, int tam) {  
    int i;

    if (!b)
        return;

    for (i = 0; i < tam; i++) {
        if (b[i]) {
            if (b[i] -> presentes)
                cjto_destroi(b[i] -> presentes);

            if (b[i] -> espera)
                fila_destroi(b[i] -> espera);

            free(b[i]);
            b[i] = NULL;
        }
    }

    free(b);
}

void destroi_missoes(struct missao_t **m, int tam) {  
    int i;

    if (!m)
        return;

    for (i = 0; i < tam; i++) {
        if (m[i]) {
            if (m[i] -> habilidades)
                cjto_destroi(m[i] -> habilidades);

            free(m[i]);
            m[i] = NULL;
        }
    }
    
    free(m);
}

void destroi_mundo(struct mundo_t **m) {
    
    if (!m)
        return;
    
    if ((*m) -> lef) 
        (*m) -> lef = fprio_destroi((*m) -> lef);

    if ((*m) -> herois)
        destroi_herois((*m) -> herois, (*m) -> n_herois);
    
    if ((*m) -> bases)
        destroi_bases((*m) -> bases, (*m) -> n_bases);

    if ((*m) -> missoes)
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

    if (!(mundo -> lef) || !(mundo -> herois) || !(mundo -> bases) || !(mundo -> missoes)) {
        destroi_mundo(&mundo);
        return NULL;
    }

    evento_cria(mundo, fim, FIM, T_FIM_DO_MUNDO, NULL, NULL);

    return mundo;
}

void configura_relogio(struct mundo_t *mundo, int tempo) {
    mundo -> relogio = tempo;
}

void percorre_lef(struct mundo_t *mundo) {
    struct evento_t *ev;
    int i, fim = 0, tipo, tempo; 

    for (i = 0; i < N_HEROIS; i++) {
        if (!(evento_inic_herois(mundo, (mundo -> herois)[i]))) {
            destroi_mundo(&mundo);
            return;
        }
    }

    for (i = 0; i < N_MISSOES; i++) {
        if (!(evento_inic_missoes(mundo, (mundo -> missoes)[i]))) {
            destroi_mundo(&mundo);
            return;
        }
    }

    configura_relogio(mundo, 0);

    while (!fim && fprio_tamanho(mundo -> lef)) {
        ev = fprio_retira(mundo -> lef, &tipo, &tempo);
        (mundo -> n_eventos)++;
        configura_relogio(mundo, ev -> tempo);
        
        switch(tipo) {
            case CHEGA: 
                evento_chega(mundo, ev);
                break;
                
            case ESPERA: 
                evento_espera(mundo, ev);
                break;
                
            case DESISTE: 
                evento_desiste(mundo, ev);
                break;
                
            case AVISA:
                evento_avisa(mundo, ev);
                break;
                
            case ENTRA: 
                evento_entra(mundo, ev);
                break;
                
            case SAI: 
                evento_sai(mundo, ev);
                break;
                
            case VIAJA: 
                evento_viaja(mundo, ev);
                break;
                
            case MORRE: 
                evento_morre(mundo, ev);
                break;

            case MISSAO:
                evento_missao(mundo, ev);
                break;
            
            case FIM:   
                fim = 1;
                evento_fim(mundo);
                break;          
        }
        free(ev);
    }
        
    destroi_mundo(&mundo);
}
